/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_expander.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 02:23:27 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/27 22:14:40 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*str_expander(char *str)
{
	char	*expanded;
	int		final_len;

	final_len = str_expander_len(str);
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
					&& ex.dquotes)))
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
					&& ex.dquotes)))
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
	ex->var_clen = ft_strlen(res);
	return (res);
}
