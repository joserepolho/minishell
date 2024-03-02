/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 15:12:44 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/01 23:35:53 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_relative_path(char *cmd)
{
	return (cmd && cmd[0] == '.' && cmd[1] == '/');
}

bool	is_absolute_path(char *cmd)
{
	return (cmd && cmd[0] == '/');
}

/**
 * @brief Check if the path is a directory and has execution permission
 * @returns `1`: valid directory + has exec perms
 * 	| `0`: not valid directory
 *  | `-1`: valid dir but no perms
 **/
int	can_access_path(char *path)
{
	int	res;

	res = access(path, F_OK);
	if (res == -1)
		return (0);
	res = access(path, X_OK);
	if (res == -1)
		return (-1);
	return (1);
}

/** 
 * @brief this function does the same as can_access_path but traverses the path
*/
int	can_path_to(char *r_path)
{
	char	**dirs;
	char	*path;
	char	*tmp;
	int		i;
	int		res;

	dirs = ft_split(r_path, '/');
	if (!dirs)
		return (0);
	path = ft_strdup("");
	i = 0;
	while (dirs[i])
	{
		tmp = path;
		path = ft_strnjoin(3, path, SLASH_STR, dirs[i]);
		if (!path)
			free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
		free(tmp);
		res = can_access_path(path);
		if (res <= 0)
			return (free(path), free_list(dirs), res);
		i++;
	}
	return (free(path), free_list(dirs), 1);
}

char	*get_env_value(t_list *envs, char *key)
{
	int	len;

	if (!(envs->content) || !key)
		return (NULL);
	len = ft_strlen(key);
	while (envs->next && ft_strncmp(key, envs->content, len))
		envs = envs->next;
	if (!(envs->content))
		return (NULL);
	return ((char *)envs->content);
}
