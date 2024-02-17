/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 03:56:01 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/16 20:02:04 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_builtin(char *s)
{
	if (!ft_strncmp(s, "cd", 2) || !ft_strncmp(s, "echo", 4)
		|| !ft_strncmp(s, "env", 3) || !ft_strncmp(s, "exit", 4)
		|| !ft_strncmp(s, "export", 6) || !ft_strncmp(s, "pwd", 3)
		|| !ft_strncmp(s, "unset", 5))
		return (1);
	return (0);
}

void	built_in(t_mini *mini, t_command *cmd)
{
	if (!ft_strncmp(cmd->cmd_name, "cd", 2))
		bi_cd(mini, cmd->args);
	else if (!ft_strncmp(cmd->cmd_name, "echo", 4))
		bi_echo(cmd->args);
	else if (!ft_strncmp(cmd->cmd_name, "env", 3))
		bi_env(mini->env_list);
	else if (!ft_strncmp(cmd->cmd_name, "export", 6))
		bi_export(mini, cmd->args);
	else if (!ft_strncmp(cmd->cmd_name, "pwd", 3))
		bi_pwd(mini);
	else if (!ft_strncmp(cmd->cmd_name, "unset", 5))
		bi_unset(mini, cmd->args);
}
