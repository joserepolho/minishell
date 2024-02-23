/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/02/15 05:01:07 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/15 05:01:07 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	bi_unset(t_mini *mini, char **av)
{
	int		i;
	size_t	len;
	t_list	*tmp;


	i = -1;
	tmp = mini->env_list;
	len = ft_strlen_eq(av[0]);
	while (av[++i])
	{
		tmp = mini->env_list;
		while (tmp)
		{
			if (!ft_strncmp(av[i], tmp->content, len))
			{
				delete_var(&(mini->env_list), tmp->content);
				break ;
			}
			tmp = tmp->next;
		}
		tmp = mini->env_list;
	}
}
