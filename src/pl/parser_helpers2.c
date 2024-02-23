/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 18:25:58 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/22 21:50:13 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*substr_expander(char *str, size_t len)
{
	char	*res;
	char	*tmp;

	tmp = ft_substr(str, 0, len);
	if (!tmp)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	res = ft_strtrim(tmp, " ");
	free(tmp);
	if (!res)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	tmp = replace_vars(res);
	if (!tmp)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	return (tmp);
}

char	*replace_vars(char *str)
{
	int		i;
	bool	in_quotes;
	bool	in_dquotes;

	i = 0;
	in_quotes = false;
	in_dquotes = false;
	while (/*(printf("replace_vars::|%c|%d|%d|\n", str[i], in_dquotes,
				in_quotes)
			|| 1) && */
			str[i] && ft_strchr(str + i, '$') != NULL)
	{
		if (str[i] == QUOTE)
			in_quotes = !in_quotes;
		else if (str[i] == DQUOTE)
			in_dquotes = !in_dquotes;
		else if (str[i] == '$' && (!in_quotes || (in_quotes && in_dquotes)))
			str = replace_var(&str, &i);
		i++;
	}
	return (str);
}

/* will replace the env var located at str[i]
		* and return the new string freeing the old one
	and incrementing i to the end of the replaced var*/
char	*replace_var(char **str, int *i)
{
	char	*var_name;
	char	*var_content;
	char	*res;
	size_t	len;

	(*i)++;
	len = 0;
	while (valid_env_char((*str)[*i + len]))
		len++;
	var_name = ft_substr(*str, *i, len);
	if (!var_name)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	var_content = get_env_var(mini()->env_list, var_name);
	res = malloc(ft_strlen(*str) + ft_strlen(var_content) - len);
	ft_strncpy(res, *str, *i - 1);
	ft_strlcat(res, var_content, ft_strlen(*str) + ft_strlen(var_content)
		- len);
	ft_strlcat(res, *str + *i + len, ft_strlen(*str) + ft_strlen(var_content)
		- len);
	free(*str);
	free(var_name);
	(*i) = (*i - 1) + ft_strlen(var_content) - 1;
	return (res);
}
/* 
	This function will remove the quotes from a string
		it will go through the whole string to find pairs of quotes
		and remove them, if there is something like this "'asd'"
		it will remove the quotes and return 'asd'
	Not implemented yet
 */
/*char	*remove_layer_quotes(char *str)
{
	(void)str;
}*/

bool	valid_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}
