/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 19:38:29 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/26 19:38:29 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_pwd(void)
{
	char	dir[PATH_MAX + 1];

	if (!getcwd(dir, PATH_MAX))
		free_shell(FAILURE_GETTING_PATH, EXIT_FAILURE, NULL, NULL);
	printf("%s\n", dir);
	mini()->command_ret = 0;
}

void	export_add(t_mini *mini, char *av)
{
	t_list	*exp;

	exp = ft_lstnew(ft_strdup((char *)av));
	ft_lstadd_back(&mini->env_list, exp);
}
