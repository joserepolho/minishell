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

int	ft_strncmp_unset(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] == '=')
			return (1);
		if (s1[i] != s2[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		i++;
	}
	return (0);
}

void	bi_unset(t_mini *mini, char **av, int j)
{
	int		i;
	size_t	len;
	t_list	*tmp;

	i = 0;
	tmp = mini->env_list;
	if (j)
		return ;
	while (av[++i])
	{
		len = ft_strlen(av[i]);
		tmp = mini->env_list;
		while (tmp)
		{
			if (!ft_strncmp_unset(av[i], tmp->content, len))
			{
				delete_var(&(mini->env_list), tmp->content);
				break ;
			}
			tmp = tmp->next;
		}
		tmp = mini->env_list;
	}
	mini->command_ret = 0;
}
