/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 05:10:41 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/14 05:10:41 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ver como deve ser arg do export
void	bi_export(t_mini *mini, char **av)
{
	int	i;

	i = 1;
	if (!av[1])
	{
		while (mini->env_list)
		{
			printf("declare -x %s\n", mini->env_list->content);
			mini->env_list = mini->env_list->next;
			return ;
		}
	}
	while (av[i])
		ft_lstadd_back(&(mini->env_list, ft_lstnew(ft_strdup(av[i++]))));
}