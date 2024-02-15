/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 03:21:49 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/15 04:53:46 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    env_update(t_mini *mini, char *pwd)
{
    char    oldpwd[PATH_MAX + 1];
	char	**exp;

    if (!getcwd(oldpwd, PATH_MAX))
        free_shell(mini, "Error\nFailure getting path!\n", 1);
	exp = malloc(4 * sizeof(char *));
	if (!exp)
		free_shell(mini, "Error\nMalloc failure!\n", 1);
	exp[0] = ft_strdup("export");
	exp[1] = ft_strjoin("PWD=", pwd);
    exp[2] = ft_strjoin("OLDPWD=", oldpwd);
    exp[3] = NULL;
    bi_export(mini, exp);
    ft_free(exp);
}

// so aceita 1 argumento. 
void    bi_cd(t_mini *mini, char **av)
{
	char	*pwd;
    char    *tmp;
    
    tmp = mini->env_list;
    if (!av[1])
    {
        while (tmp)
        {
            if (!ft_strncmp(tmp->content ,"HOME=", 5))
                break ;
            tmp = tmp->next;
        }
		pwd = tmp->content;
        if (chdir(tmp->content))
            free_shell(mini, "Error\nDirectory change failure!\n", 1);
    }
    else
    {
		pwd = av[1];
        if (chdir(av[1]))
            free_shell(mini, "Error\nDirectory change failure!\n", 1);
    }
    env_update(mini, pwd);
}
