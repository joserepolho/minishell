/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 18:22:28 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/23 05:59:55 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands(t_command *commands)
{
	t_command	*tmp;

	while (commands != NULL)
	{
		tmp = commands->next;
		if (commands->cmd_name)
			free(commands->cmd_name);
		if (commands->in.file)
			free(commands->in.file);
		if (commands->out.file)
			free(commands->out.file);
		if (commands->args)
			free_list(commands->args);
		free(commands);
		commands = tmp;
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
		free(list[i]);
		i++;
	}
	free(list);
}

// mudei a funcao para podermos dar free de alguma variavel que tenhamos alocado
void	free_shell(char *err, int status, void (*cleanup_func)(void *),
		void *free_arg)
{
	t_mini	*m;

	m = mini();
	#ifdef DEBUG
		printf("freeing shell\n");
	#endif
	if (free_arg != NULL && cleanup_func != NULL)
		cleanup_func(free_arg);
	reset_mini(m);
	if (m->input.pip)
	{
		close(m->input.pip[0]);
		close(m->input.pip[1]);
	}
	if (m->env_list)
		ft_lstclear(&(m->env_list), free_content);
	rl_clear_history();
	if (err)
		write(2, err, ft_strlen(err));
	close(0);
	close(1);
	exit(status);
}

void	free_commands_wrapper(void *arg)
{
	t_command	*cmd;

	cmd = (t_command *)arg;
	free_commands(cmd);
}
