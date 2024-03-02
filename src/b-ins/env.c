/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
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
	mini()->command_ret = 0;
}

char	*get_env_var(t_list *env_list, char *var)
{
	int	i;

	if (!ft_strncmp(var, "?", 1))
		return (ft_itoa(mini()->command_ret));
	i = ft_strlen(var);
	while (env_list)
	{
		if (ft_strncmp((char *)env_list->content, var, i) == 0)
			return (ft_strchr((char *)env_list->content, '=') + 1);
		env_list = env_list->next;
	}
	return ("");
}

/*
**	Checks if the string is a valid environment variable name
**	@param str: the string to be checked
**	@param is_entry: if the string to check should be a entry
**	@return length of the name of the variable if valid,
		-1 if invalid, if is_entry=true: 0 if there's no = and still valid name
 */
int	valid_env_var_name(char *str, bool is_entry)
{
	int		namelen;
	int		i;

	namelen = ft_strlen_eq(str);
	i = 0;
	if ((!is_entry && str[namelen] == '=')
		|| namelen == 0 || ft_isdigit(str[i]))
		return (mini()->command_ret = 1, -1);
	while (str[i])
	{
		if (is_entry && str[i] == '=')
			return (mini()->command_ret = 1, i);
		if (!valid_env_char(str[i]) || (!is_entry && str[i] == '='))
			return (mini()->command_ret = 1, -1);
		i++;
	}
	if (is_entry && str[namelen] != '=')
	{
		mini()->command_ret = 1;
		return (0);
	}
	return (1);
}
