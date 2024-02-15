/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 05:01:07 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/15 05:01:07 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_unset(t_mini *mini, char **av)
{
	int		i;
	int		j;
	t_list	*tmp;
	int		size_to_eq;

	i = 0;
	j = 0;
	tmp = mini->env_list;
	while (av[++i])
	{
		tmp = mini->env_list;
		while (tmp)
		{
			if (!ft_strncmp(av[i], tmp->content,
				ft_strlen_eq(tmp->content)))
			{
				j = ft_strlen(tmp->content);
				if (av[j + 1])
				{
					delete_var(&(mini->env_list), tmp->content);
					ft_lstadd_back(&(mini->env_list, 
									ft_lstnew(ft_strdup(av[i]))));
				}
				else
				{
					delete_var(&(mini->env_list), tmp->content);
					break ;
				}
			}
			tmp = tmp->next;
		}
	}
}
