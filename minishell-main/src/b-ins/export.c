/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 20:17:40 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/05 20:17:40 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	show_export_eq(t_list *tmp, char *var, int i)
{
	char	*val;
	char	*vall;

	val = ft_strdup((char *)tmp->content);
	vall = val;
	vall += i + 1;
	printf("declare -x %s=\"%s\"\n", var, vall);
	free(val);
}

void	show_export(t_list *env_list, char **av)
{
	int		i;
	t_list	*tmp;
	char	*var;

	i = 0;
	if (av[1])
		return ;
	tmp = env_list;
	tmp = sort_list(tmp);
	while (tmp)
	{
		i = ft_strlen_eq(tmp->content);
		var = ft_strdup((char *)tmp->content);
		var[i] = '\0';
		if (ft_strnstr(tmp->content, "=", ft_strlen(tmp->content)))
			show_export_eq(tmp, var, i);
		else
			printf("declare -x %s\n", var);
		free(var);
		tmp = tmp->next;
	}
}

int	delete_if_needed(t_list **env_list, char *var, int len)
{
	t_list	*tmp;

	tmp = *env_list;
	while (tmp)
	{
		if (ft_strlen_eq(tmp->content) == len
			&& !ft_strncmp(tmp->content, var, len))
		{
			if (!ft_strnstr(var, "=", ft_strlen(var)))
				return (0);
			delete_var(env_list, tmp->content);
			break ;
		}
		tmp = tmp->next;
	}
	return (1);
}

void	bi_export(t_mini *mini, char **av, int j)
{
	int		i;
	int		k;
	int		res;

	show_export(mini->env_list, av);
	i = 0;
	while (av[++i])
	{
		res = valid_env_var_name(av[i], true);
		if (j)
			return ;
		if (res == 0)
			continue ;
		else if (res == -1)
		{
			error_msg(NOT_VALID_IDENT, av[i]);
			continue ;
		}
		k = delete_if_needed(&mini->env_list, av[i], res);
		if (k)
			export_add(mini, av[i]);
	}
	mini->command_ret = 0;
}
