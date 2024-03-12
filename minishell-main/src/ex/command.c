/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 22:40:19 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/02 04:00:01 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prepare_cmd(t_command *cmd, t_command *prev, t_command *next)
{
	if (prev)
		cmd->std.in = prev->pip[0];
	else
		cmd->std.in = STDIN_FILENO;
	if (next)
	{
		if (pipe(cmd->pip) < 0)
			free_shell(PIPE_ERROR, EXIT_FAILURE, NULL, NULL);
		cmd->std.out = cmd->pip[1];
	}
	else
		cmd->std.out = STDOUT_FILENO;
}

void	prepare_cmd_for_child(t_command *cmd, t_command *prev)
{
	if (cmd->std.in != STDIN_FILENO)
	{
		dup2(cmd->std.in, STDIN_FILENO);
		close(cmd->std.in);
	}
	if (cmd->std.out != STDOUT_FILENO)
	{
		dup2(cmd->std.out, STDOUT_FILENO);
		close(cmd->std.out);
	}
	if (prev && prev->pip[0] != STDIN_FILENO)
		close(prev->pip[0]);
	if (cmd->next)
		close(cmd->pip[0]);
}
