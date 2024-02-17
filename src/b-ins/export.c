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

void	delete_var(t_list **head, void *node_to_del)
{
	t_list	*current;
	t_list	*previous;

	current = *head;
	previous = NULL;
	while (current)
	{
		if (!ft_strncmp(current->content, node_to_del, ft_strlen(node_to_del)))
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
	t_list	*dtmp;
	t_list	*exp;

	i = -1;
	tmp = mini->env_list;
	if (!av[0])
	{
		while (tmp)
		{
			printf("declare -x %p\n", tmp->content);
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
				dtmp = tmp->content;
				delete_var(&(mini->env_list), dtmp->content);
			}
			tmp = tmp->next;
		}
		exp = ft_lstnew(ft_strdup((char *)av[i]));
		ft_lstadd_back(&(mini->env_list), exp);
	}
}
