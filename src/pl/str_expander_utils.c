/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_expander_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 19:21:41 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/08 17:08:21 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	expand_command_gate(t_command *cmd)
{
	int	i;

	i = -1;
	while (cmd->args[++i])
	{
		if (!ft_strncmp(cmd->args[i], "..", 3))
			return (false);
	}
	if ((cmd->args && cmd->args[1] && !ft_strncmp(cmd->args[1], "$vari", 6)))
		return (false);
	return (true);
}

bool	expand_command(t_command *cmd)
{
	if (!expand_command_gate(cmd))
		return (true);
	expand_args(cmd);
	if (!expand_redirs(cmd))
		return (cmd->cmd_name = NULL, false);
	if (cmd->args && cmd->args[0] != NULL)
	{
		if (ft_strlen(cmd->args[0]) == 0)
			return (error_msg_ret(CMD_NOT_FOUND, "''", CMD_NOT_FOUND_RET),
				false);
		cmd->expanded = true;
		if (if_builtin(cmd->args[0]))
		{
			cmd->cmd_name = ft_strdup(cmd->args[0]);
			if (!cmd->cmd_name)
				free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
			return (true);
		}
		cmd->cmd_name = get_cmd_path(cmd->args[0]);
		if (cmd->cmd_name == NULL)
			return (false);
	}
	return (true);
}

void	expand_args(t_command *cmd)
{
	size_t	i;
	char	*expanded;

	i = 0;
	while (cmd->args && cmd->args[i])
	{
		if (cmd->args[i] && cmd->args[i][0] == '\0')
			goto continue_while;
		expanded = str_expander(cmd->args[i]);
		if (ft_strlen(expanded) < ft_strlen(cmd->args[i]))
		{
			ft_strlcpy(cmd->args[i], expanded, ft_strlen(expanded) + 1);
			free(expanded);
			goto continue_while;
		}
		free(cmd->args[i]);
		cmd->args[i] = expanded;
		continue_while:
			i++;
	}
}

bool	expand_redirs(t_command *cmd)
{
	t_redir	*redir;
	char	*expanded;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->file && redir->file[0] == '\0')
		{
			redir = redir->next;
			continue ;
		}
		expanded = str_expander(redir->file);
		free(redir->file);
		redir->file = expanded;
		if (redir->type == RED_IN)
			if (access(redir->file, F_OK | R_OK) != 0)
				return (error_msg_ret(FD_NOT_FOUND, redir->file, EXIT_FAILURE),
					false);
		redir = redir->next;
	}
	return (true);
}
