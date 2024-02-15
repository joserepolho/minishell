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

int	ft_strlen_eq(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	return (i);
}

void	free_content(void *content)
{
	free(content);
}

void	delete_var(t_list **head, t_list node_to_del)
{
	t_list	*current;
	t_list	*previous;

	current = *head;
	previous = NULL;
	while (current)
	{
		if (current->content == node_to_del)
		{
			if (previous)
				previous->next = current->next;
			else
				*head = current->next;
			ft_lstdelone(current, free_content);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

// ver como deve ser arg do export
void	bi_export(t_mini *mini, char **av)
{
	int		i;
	t_list	*tmp;

	i = 0;
	tmp = mini->env_list;
	if (!av[1])
	{
		while (tmp)
		{
			printf("declare -x %s\n", tmp->content);
			tmp = tmp->next;
		}
		return ;
	}
	tmp = mini->env_list;
	while (av[++i])
	{
		while (tmp)
		{
			if (!ft_strncmp(av[i], tmp->content, ft_strlen_eq(tmp->content)))
			{
				delete_var(&(mini->env_list), tmp->content);
				break ;
			}
			tmp = tmp->next;
		}
	}
	ft_lstadd_back(&(mini->env_list, ft_lstnew(ft_strjoin("declare -x",
					av[i]))));
}
