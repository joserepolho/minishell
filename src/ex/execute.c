/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 02:29:23 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/21 18:23:02 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cmd_path(char **ev)
{
	while (ft_strncmp("PATH", *ev, 4))
		ev++;
	return (*ev + 5);
}

char	*get_path(char *cmd, char **ev)
{
	int		i;
	char	**paths;
	char	*path;
	char	*tmp;

	i = 0;
	paths = ft_split(cmd_path(ev), ':');
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		tmp = path;
		path = ft_strjoin(path, cmd);
		free(tmp);
		if (!access(path, F_OK | X_OK))
		{
			free_list(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_list(paths);
	return (0);
}

void	execution(t_mini *mini, t_command *cmd, char **ev)
{
	char	*path;

	if (if_builtin(cmd->cmd_name))
		built_in(mini, cmd);
	else
	{
		path = get_path(cmd->cmd_name, ev);
		if (!path || execve(path, cmd->args, ev) < 0)
		{
			error_msg(CMD_NOT_FOUND, cmd->cmd_name);
			if (!path)
				free(path);
			return ;
		}
		free(path);
	}
}
