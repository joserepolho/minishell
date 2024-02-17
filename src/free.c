/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 18:22:28 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/17 21:55:55 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_atoi_exit(const char *nptr)
{
	int		r;
	int		s;
	char	*d;

	d = (char *) nptr;
	r = 0;
	s = 1;
	if (!nptr)
		return (0);
	while (*d == ' ' || *d == '\n' || *d == '\t' || *d == '\v'
		|| *d == '\f' || *d == '\r')
		d++;
	if (*d == '-' || *d == '+')
	{
		if (*d == '-')
			s = -1;
		d++;
	}
	while (*d >= '0' && *d <= '9')
	{
		r = r * 10 + *d - '0';
		d++;
	}
	return (r * s);
}

void	free_commands(t_command *commands)
{
	t_command	*tmp;

	tmp = commands;
	while (tmp != NULL)
	{
		if (tmp->cmd_name)
			free(tmp->cmd_name);
		if (tmp->in.file)
			free(tmp->in.file);
		if (tmp->out.file)
			free(tmp->out.file);
		if (tmp->args)
			free_list(tmp->args);
		commands = tmp;
		tmp = tmp->next;
	}
	free(commands);
}

void	free_list(char **list)
{
	int	i;

	i = 0;
	if (list == NULL)
	{
		return ;
	}
	while (list[i] != NULL)
	{
		if (list[i])
			free(list[i]);
		i++;
	}
	free(list);
}

void	free_mini(t_mini *mini)
{
	reset_mini(mini);
}

// adicionei este free para os frees que ja tenho na execution. pode se manter os dois
void    free_shell(t_mini *mini, char *err, char *status)
{
	int	exitt;

	exitt = 0;
	if (status)
		exitt = ft_atoi_exit(status);
	if (mini->input.raw_line)
		free(mini->input.raw_line);
	if (mini->commands)
		free_commands(mini->commands);
	if (mini->input.pip)
	{
		close(mini->input.pip[0]);
		close(mini->input.pip[1]);
	}
	if (mini->env_list)
		ft_lstclear(&(mini->env_list), free_content);
	if (err)
		write(2, err, ft_strlen(err));
	if (exitt)
	{
		rl_clear_history();
		close(0);
		close(1);
		exit(exitt);
	}
}
