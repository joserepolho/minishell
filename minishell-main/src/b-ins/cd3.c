/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd3.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 03:16:07 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/07 03:52:08 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd_noarg_tilde(char *av, int p, char *oldpwd)
{
	t_list	*tmp;

	tmp = mini()->env_list;
	while (tmp)
	{
		if (!ft_strncmp(tmp->content, "HOME=", 5))
			break ;
		tmp = tmp->next;
	}
	if (!tmp)
		return (2);
	mini()->home_tmp = tmp->content;
	if (access(mini()->home_tmp + 5, F_OK | R_OK) == -1 || !mini()->home_tmp)
	{
		error_msg_ret(FD_NOT_FOUND, "cd", EXIT_FAILURE);
		return (0);
	}
	if (!p)
	{
		chdir(mini()->home_tmp + 5);
		env_update(mini(), oldpwd);
	}
	if (!av || (av && !ft_strncmp(av, "$vari", 5)))
		return (0);
	return (1);
}

char	*delete_until_char(char *str, char c)
{
	char	*ptr;

	if (!str)
		return (NULL);
	ptr = str + ft_strlen(str) - 1;
	while (ptr >= str && *ptr != c)
		ptr--;
	if (*ptr == c)
		ptr++;
	*ptr = '\0';
	return (str);
}

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

char	*ft_strdup_oldpwd(const char *s, int *i)
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
	*i = 3;
	return (d);
}

int	cd_noarg_return(char *av, int p, char *oldpwd)
{
	int	j;

	j = cd_noarg_tilde(av, p, oldpwd);
	if (!j)
		return (0);
	else if (j == 2)
	{
		error_msg("HOME not set: ", "cd");
		return (0);
	}
	return (1);
}
