/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 18:22:28 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/03 05:24:16 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands(t_command *commands)
{
	t_command	*tmp;

	while (commands != NULL)
	{
		tmp = commands->next;
		if (commands->cmd_name && (commands->redirs
			&& commands->redirs->red_in_not_found) == 1)
			free(commands->cmd_name);
		if (commands->redirs && commands->redirs->red_in_not_found == 1)
			commands->redirs->red_in_not_found = 0;
		if (commands->redirs)
			free_redirs(commands->redirs);
		if (commands->args)
			free_list(commands->args);
		free(commands);
		commands = tmp;
	}
}

void	free_redirs(t_redir *redirs)
{
	t_redir	*tmp;

	while (redirs != NULL)
	{
		tmp = redirs->next;
		if (redirs->file)
			free(redirs->file);
		free(redirs);
		redirs = tmp;
	}
}

void	free_list(char **list)
{
	int	i;

	i = 0;
	if (list == NULL)
		return ;
	while (list[i] != NULL)
	{
		if (list && list[i] && list[i][0])
			free(list[i]);
		i++;
	}
	if (list)
		free(list);
}

// mudei a funcao para podermos dar free de alguma variavel que tenhamos alocado
void	free_shell(char *err, int status, void (*cleanup_func)(void *),
		void *free_arg)
{
	t_mini	*m;

	m = mini();
	if (free_arg != NULL && cleanup_func != NULL)
		cleanup_func(free_arg);
	reset_mini(m);
	if (m->input.pip)
	{
		close(m->input.pip[0]);
		close(m->input.pip[1]);
	}
	if (m->env_list)
		ft_lstclear(&(m->env_list), free);
	rl_clear_history();
	if (err)
		write(STDERR_FILENO, err, ft_strlen(err));
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	exit(status);
}

void	free_commands_wrapper(void *arg)
{
	t_command	*cmd;

	cmd = (t_command *)arg;
	free_commands(cmd);
}
