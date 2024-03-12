/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 03:14:55 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/07 01:35:11 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	path_with_dots_2(char **pths, char *oldpwd, int *j, int p)
{
	int		i;
	char	*t_oldpwd;

	t_oldpwd = ft_strdup_oldpwd(oldpwd, &i);
	while (pths[++(*j)])
	{
		getcwd(t_oldpwd, PATH_MAX);
		if (pths[*j][0] == '.' && pths[*j][1] == '.' && pths[*j][2] == '/')
		{
			if (!dot_handler(t_oldpwd, oldpwd, pths, p))
				return (0);
			clean_until_dots(pths, j, &i);
		}
		else
		{
			if (access(pths[*j], F_OK | R_OK) == -1)
			{
				clean_after_access(oldpwd, pths, t_oldpwd, &i);
				return (0);
			}
			non_dot_chdir(pths, oldpwd, j, p);
		}
	}
	free(t_oldpwd);
	return (i);
}

void	non_dot_chdir(char **pths, char *oldpwd, int *j, int p)
{
	if (!p)
		chdir(pths[*j]);
	env_update(mini(), oldpwd);
}

void	clean_until_dots(char **pths, int *j, int *p)
{
	int	i;

	i = -1;
	while (pths[++i])
	{
		if (!ft_strncmp(pths[*j], "..", 3))
			break ;
		free(pths[i]);
		pths[i] = NULL;
	}
	*p = 2;
}

void	clean_after_access(char *oldpwd, char **pths, char *t_oldpwd, int *i)
{
	*i = 1;
	chdir(oldpwd);
	free_list(pths);
	free(t_oldpwd);
	error_msg_ret(FD_NOT_FOUND, mini()->commands->args[1], EXIT_FAILURE);
}

int	dot_handler(char *t_oldpwd, char *oldpwd, char **pths, int p)
{
	t_oldpwd = delete_until_char(t_oldpwd, '/');
	if (access(t_oldpwd, F_OK | R_OK) == -1)
	{
		chdir(oldpwd);
		free_list(pths);
		free(t_oldpwd);
		error_msg_ret(FD_NOT_FOUND, t_oldpwd, EXIT_FAILURE);
		return (0);
	}
	if (!p)
		chdir(t_oldpwd);
	env_update(mini(), oldpwd);
	return (1);
}
