/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_expander_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 19:21:41 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/03 05:23:16 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	expand_command(t_command *cmd, char **ev)
{
	expand_args(cmd);
	if (!expand_redirs(cmd))
		return (false);
	(void)ev;
	if (cmd->args && cmd->args[0] != NULL)
	{
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
		{
			i++;
			continue ;
		}
		expanded = str_expander(cmd->args[i]);
		free(cmd->args[i]);
		cmd->args[i] = expanded;
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
			{
				return (error_msg_ret(FD_NOT_FOUND, redir->file, EXIT_FAILURE),
					false);
				redir->red_in_not_found = 1;
			}
		redir = redir->next;
	}
	return (true);
}
