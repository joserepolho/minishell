/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 01:50:53 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/14 01:50:53 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_env(t_list *env_list)
{
	while (env_list)
	{
		printf("%s\n", (char *)env_list->content);
		env_list = env_list->next;
	}
}

char	*get_env_var(t_list *env_list, char *var)
{
	int	i;

	i = ft_strlen(var);
	while (env_list)
	{
		if (ft_strncmp((char *)env_list->content, var, i) == 0)
			return (ft_strchr((char *)env_list->content, '=') + 1);
		env_list = env_list->next;
	}
	return ("");
}
