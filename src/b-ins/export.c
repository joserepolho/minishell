/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
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
			ft_lstdelone(current, free);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

void	show_export(t_list *env_list, char **av)
{
	t_list	*tmp;

	if (av[1])
		return ;
	tmp = env_list;
	while (tmp)
	{
		printf("declare -x %s\n", (char *)tmp->content);
		tmp = tmp->next;
	}
}

void	delete_if_needed(t_list **env_list, char *var, int len)
{
	t_list	*tmp;

	tmp = *env_list;
	while (tmp)
	{
		if (!ft_strncmp(var, tmp->content, len))
		{
			delete_var(env_list, tmp->content);
			return ;
		}
		tmp = tmp->next;
	}
}

void	bi_export(t_mini *mini, char **av)
{
	int		i;
	int		res;
	t_list	*exp;

	show_export(mini->env_list, av);
	i = 0;
	while (av[++i])
	{
		res = valid_env_var_name(av[i], true);
		if (res == 0)
			continue ;
		else if (res == -1)
			return ;
		delete_if_needed(&mini->env_list, av[i], res);
		exp = ft_lstnew(ft_strdup((char *)av[i]));
		ft_lstadd_back(&mini->env_list, exp);
	}
	mini->command_ret = 0;
}
