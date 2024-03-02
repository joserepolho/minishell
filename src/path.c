/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 14:36:05 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/02 00:04:43 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 *  @brief check relative or absolute path and return copy
 **/
char	*get_roa_path(char *cmd)
{
	int		res;

	res = can_path_to(cmd);
	if (res == -1)
		return (error_msg_ret(CMD_INSUF_PERMS, cmd, CMD_INSUF_PERMS_RET), NULL);
	else if (res == 0)
		return (error_msg_ret(CMD_NOT_FOUND, cmd, CMD_NOT_FOUND_RET), NULL);
	return (cmd);
}

/**
 * @brief Search on PATH env variable for cmd
 **/
char	*get_envpath_cmd(char *cmd)
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	int		i;

	path_var = get_env_value(mini()->env_list, "PATH");
	if (path_var && ft_strlen(path_var) == 0)
		return (NULL);
	path_var += 5;
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = ft_strnjoin(3, paths[i], SLASH_STR, cmd);
		if (!full_path)
			free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
		if (can_access_path(full_path) == 1)
			return (free_list(paths), full_path);
		free(full_path);
		i++;
	}
	free_list(paths);
	return (error_msg_ret(CMD_NOT_FOUND, cmd, CMD_NOT_FOUND_RET), NULL);
}

char	*path_add_home_prefix(char *cmd)
{
	char	*home_absp;
	char	*res;

	home_absp = get_env_value(mini()->env_list, "HOME");
	if (home_absp)
	{
		home_absp += 5;
		res = ft_strnjoin(2, home_absp, cmd + 1);
		if (!res)
			free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
		return (res);
	}
	return (ft_strdup(cmd + 1));
}

char	*get_relative_path(char *cmd)
{
	char	cwd[PATH_MAX + 1];
	char	*path;
	char	*res;

	if (!getcwd(cwd, PATH_MAX))
		free_shell(FAILURE_GETTING_PATH, EXIT_FAILURE, NULL, NULL);
	path = ft_strnjoin(3, cwd, SLASH_STR, cmd + 2);
	if (!path)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
	res = get_roa_path(path);
	if (!res)
		return (free(path), NULL);
	return (path);
}

/**
 *  @brief get path and validate it from cmd
 **/
char	*get_cmd_path(char *cmd)
{
	int		i;
	char	*path;
	bool	home_added;

	i = 0;
	path = NULL;
	home_added = false;
	if (is_absolute_path(cmd))
		i++;
	if (cmd && *cmd == TILDE)
	{
		cmd = path_add_home_prefix(cmd);
		home_added = true;
	}
	if (is_relative_path(cmd))
	{
		if (home_added)
			free(cmd);
		return (get_relative_path(cmd));
	}
	else if (is_absolute_path(cmd))
		path = get_roa_path(cmd);
	else
		path = get_envpath_cmd(cmd);
	if (home_added)
		free(cmd);
	if (i)
		return (ft_strdup(path));
	return (path);
}
