/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 03:56:01 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/06 03:44:51 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_builtin(char *s)
{
	if (!ft_strncmp(s, "cd", 3) || !ft_strncmp(s, "echo", 5)
		|| !ft_strncmp(s, "env", 4) || !ft_strncmp(s, "exit", 5)
		|| !ft_strncmp(s, "export", 7) || !ft_strncmp(s, "pwd", 5)
		|| !ft_strncmp(s, "unset", 6) || !ft_strncmp(s, "exit", 5))
		return (1);
	return (0);
}

int	if_builtin_epe(char *s)
{
	if (!ft_strncmp(s, "echo", 5) || !ft_strncmp(s, "env", 4)
		|| !ft_strncmp(s, "pwd", 5))
		return (1);
	return (0);
}

void	built_in(t_mini *mini, t_command *cmd, int j)
{
	if (!ft_strncmp(cmd->cmd_name, "cd", 2))
		bi_cd(mini, cmd->args, j);
	else if (!ft_strncmp(cmd->cmd_name, "echo", 4))
		bi_echo(cmd->args);
	else if (!ft_strncmp(cmd->cmd_name, "env", 3))
		bi_env(mini->env_list);
	else if (!ft_strncmp(cmd->cmd_name, "export", 6))
		bi_export(mini, cmd->args, j);
	else if (!ft_strncmp(cmd->cmd_name, "pwd", 3))
		bi_pwd();
	else if (!ft_strncmp(cmd->cmd_name, "unset", 5))
		bi_unset(mini, cmd->args, j);
	else if (!ft_strncmp(cmd->cmd_name, "exit", 4)
		&& !mini->exit_unavailability)
		bi_exit(mini, cmd->args, cmd->next != NULL);
}

t_list	*sort_list(t_list *lst)
{
	char	*swap;
	t_list	*tmp;

	tmp = lst;
	while (lst->next != NULL)
	{
		if (ft_strncmp(lst->content, lst->next->content,
				ft_strlen(lst->content) + 1) > 0)
		{
			swap = lst->content;
			lst->content = lst->next->content;
			lst->next->content = swap;
			lst = tmp;
		}
		else
			lst = lst->next;
	}
	lst = tmp;
	return (lst);
}

int	ft_strlen_eq(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	return (i);
}
