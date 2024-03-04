/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 03:21:49 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/03 01:22:20 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_update(t_mini *mini, char *oldpwd)
{
	char	pwd[PATH_MAX + 1];
	char	*f_pwd;
	char	*f_oldpwd;
	char	**exp;

	if (!getcwd(pwd, PATH_MAX))
		free_shell(FAILURE_GETTING_PATH, EXIT_FAILURE, NULL, NULL);
	f_pwd = pwd;
	f_oldpwd = oldpwd;
	exp = malloc(4 * sizeof(char *));
	if (!exp)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
	exp[0] = ft_strdup("export");
	exp[1] = ft_strjoin("PWD=", f_pwd);
	exp[2] = ft_strjoin("OLDPWD=", f_oldpwd);
	exp[3] = NULL;
	bi_export(mini, exp, 0);
	free_list(exp);
}

char	*ft_strdup_oldpwd(const char *s)
{
	char	*d;
	size_t	c;

	d = malloc(PATH_MAX + 1);
	c = 0;
	if (d == NULL)
		return (NULL);
	while (s[c])
	{
		d[c] = s[c];
		c++;
	}
	d[c] = '\0';
	return (d);
}

// so aceita 1 argumento.
void	bi_cd(t_mini *mini, char **av, int p)
{
	char	oldpwd[PATH_MAX + 1];
	t_list	*tmp;
	char	*tmp_0;
	char	*tmp_oldpwd;
	char	*t_oldpwd;
	char	**split_oldpwd;
	char	**tmp_split;
	char	**pths;
	char	*final_oldpwd;
	char	*f_tmp;
	char	*f_tmp2;
	int		i;
	int		j;
	int		k;
	int		l;

	i = -1;
	j = 0;
	k = 0;
	l = 0;
	if (!getcwd(oldpwd, PATH_MAX))
		free_shell(FAILURE_GETTING_PATH, EXIT_FAILURE, NULL, NULL);
	tmp = mini->env_list;
	if (av[2])
	{
		error_msg_ret(TOO_MANY_ARGS, "cd", EXIT_FAILURE);
		return ;
	}
	if (!av[1] || (av[1][0] == TILDE))
	{
		i += 2;
		while (tmp)
		{
			if (!ft_strncmp(tmp->content, "HOME=", 5))
				break ;
			tmp = tmp->next;
		}
		tmp_0 = (char *)tmp->content;
		if (access(tmp_0 + 5 , F_OK | R_OK) == -1)
		{
			error_msg_ret(FD_NOT_FOUND, "cd", EXIT_FAILURE);
			return ;
		}
		else if (chdir(tmp_0 + 5) && !p)
		{
			error_msg_ret(FD_NOT_FOUND, "cd", EXIT_FAILURE);
			return ;
		}
	}
	if (av[1] && ((av[1] && av[1][0] != TILDE) || (av[1][0] == TILDE && av[1][1])))
	{
		while (av[1][++i])
		{
			if (av[1][i] == '.' || av[1][0] == TILDE)
			{
				if (av[1][i + 1] == '.' || av[1][0] == TILDE)
				{
					j = -1;
					pths = ft_split(av[1], '/');
					if (!ft_strncmp(pths[0], "~", 1))
						j = 0;
					t_oldpwd = ft_strdup_oldpwd(oldpwd);
					while (pths[++j])
					{
						if (!getcwd(t_oldpwd, PATH_MAX))
							free_shell(FAILURE_GETTING_PATH, EXIT_FAILURE, NULL, NULL);
						tmp_oldpwd = t_oldpwd;
						split_oldpwd = ft_split(tmp_oldpwd, '/');
						if (pths[j][0] == '.')
						{
							final_oldpwd = ft_strdup(SLASH_STR);
							tmp_split = split_oldpwd;
							k = 0;
							while (tmp_split[k])
								k++;
							k--;
							l = 0;
							while (l < k)
							{
								f_tmp = ft_strjoin(final_oldpwd, tmp_split[l]);
								free(final_oldpwd);
								final_oldpwd = f_tmp;
								if (l + 1 != k)
								{
									f_tmp2 = ft_strjoin(final_oldpwd, SLASH_STR);
									free(final_oldpwd);
									final_oldpwd = f_tmp2;
								}
								l++;
							}
							if (access(final_oldpwd, F_OK | R_OK) == -1)
							{
								chdir(oldpwd);
								free(final_oldpwd);
								free_list(split_oldpwd);
								free_list(pths);
								free(t_oldpwd);
								error_msg_ret(FD_NOT_FOUND, "cd", EXIT_FAILURE);
								return ;
							}
							else if (chdir(final_oldpwd) && !p)
							{
								error_msg_ret(FD_NOT_FOUND, "cd", EXIT_FAILURE);
								return ;
							}
							free(final_oldpwd);
							env_update(mini, oldpwd);
							free_list(split_oldpwd);
						}
						else
						{
							if (access(pths[j], F_OK | R_OK) == -1)
							{
								chdir(oldpwd);
								free_list(split_oldpwd);
								free_list(pths);
								free(t_oldpwd);
								error_msg_ret(FD_NOT_FOUND, "cd", EXIT_FAILURE);
								return ;
							}
							else if (chdir(pths[j]) && !p)
							{
								error_msg_ret(FD_NOT_FOUND, "cd", EXIT_FAILURE);
								return ;
							}
							env_update(mini, oldpwd);
							free_list(split_oldpwd);
						}
					}
					if (!pths[j])
					{
						free_list(pths);
						break ;
					}
				}
				else if (av[1][i + 1] == SLASH)
					i += 2;
			}
		}
		if (!j)
		{
			if (access(av[1], F_OK | R_OK) == -1)
			{
				error_msg_ret(FD_NOT_FOUND, "cd", EXIT_FAILURE);
				return ;
			}
			else if (chdir(av[1]) && !p)
			{
				error_msg_ret(FD_NOT_FOUND, "cd", EXIT_FAILURE);
				return ;
			}
			env_update(mini, oldpwd);
		}
	}
	mini->command_ret = 0;
}
