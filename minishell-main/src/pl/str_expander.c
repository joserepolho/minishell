/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 02:23:27 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/08 00:14:05 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_expander(char *str)
{
	char	*expanded;
	char	*tmp;
	int		final_len;

	final_len = str_expander_len(str);
	if (final_len == 0)
	{
		if (has_char_in_set(str, "\"\'"))
		{
			tmp = ft_strdup("");
			if (!tmp)
				free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
			return (tmp);
		}
		else
			return (NULL);
	}
	expanded = malloc(final_len + 1);
	if (!expanded)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	memset(expanded, 0, final_len + 1);
	expand_vars(str, expanded, final_len);
	return (expanded);
}

void	expand_vars(char *str, char *expanded, int len)
{
	t_str_ex	ex;

	memset(&ex, 0, sizeof(t_str_ex));
	while (str[ex.i])
	{
		if ((str[ex.i] == QUOTE && ex.dquotes) || (str[ex.i] == DQUOTE
				&& ex.quotes))
			ft_strlcat(expanded, str + ex.i, ft_strlen(expanded) + 2);
		else if (str[ex.i] == QUOTE && !ex.dquotes)
			ex.quotes = !ex.quotes;
		else if (str[ex.i] == DQUOTE && !ex.quotes)
			ex.dquotes = !ex.dquotes;
		else if (str[ex.i] == ENV_VAR && (!ex.quotes || (ex.quotes
					&& ex.dquotes)) && str_starts_with_env_var(str + ex.i))
		{
			ft_strlcat(expanded, str_expander_var_len(&ex, str), len + 1);
			free_assign_null((void **)&ex.var);
			continue ;
		}
		else
			ft_strlcat(expanded, str + ex.i, ft_strlen(expanded) + 2);
		ex.i++;
	}
	expanded[len] = '\0';
}

int	str_expander_len(char *str)
{
	t_str_ex	ex;

	memset(&ex, 0, sizeof(t_str_ex));
	while (str[ex.i])
	{
		if ((str[ex.i] == QUOTE && ex.quotes && !ex.dquotes)
			|| (str[ex.i] == DQUOTE && ex.dquotes && !ex.quotes))
			ex.len -= 2;
		if (str[ex.i] == QUOTE && !ex.dquotes)
			ex.quotes = !ex.quotes;
		else if (str[ex.i] == DQUOTE && !ex.quotes)
			ex.dquotes = !ex.dquotes;
		else if (str[ex.i] == ENV_VAR && (!ex.quotes || (ex.quotes
					&& ex.dquotes)) && str_starts_with_env_var(str + ex.i))
		{
			str_expander_var_len(&ex, str);
			ex.len += ex.var_clen;
			free_assign_null((void **)&ex.var);
			continue ;
		}
		ex.len++;
		ex.i++;
	}
	return (ex.len);
}

/* @returns - the env variable content */
char	*str_expander_var_len(t_str_ex *ex, char *str)
{
	char	*tmp;
	char	*res;

	ex->var_len = 0;
	if (str[ex->i] == ENV_VAR)
		ex->i++;
	if (str[ex->i] == ENV_VAR)
		ex->var_len += 1;
	while (str[ex->i] && valid_env_char(str[ex->i]))
	{
		ex->var_len++;
		ex->i++;
	}
	if (str[ex->i] != ENV_Q)
	{
		tmp = ft_substr(str, ex->i - ex->var_len, ex->var_len);
		res = get_env_var(mini()->env_list, tmp);
		free(tmp);
	}
	else
	{
		res = get_env_var(mini()->env_list, str + ex->i++);
		ex->var = res;
	}
	return (ex->var_clen = ft_strlen(res), res);
}

bool	str_starts_with_env_var(char *str)
{
	int	i;

	i = 0;
	if (str[i] == ENV_VAR)
		i++;
	else
		return (false);
	if (str[i] == ENV_Q)
		return (true);
	while (str[i] && valid_env_char(str[i]))
		i++;
	return (1);
}
