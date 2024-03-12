/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_str_expander.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 21:34:19 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/04 21:36:11 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_vars_hd(char *str, char *expanded, int len)
{
	t_str_ex	ex;

	memset(&ex, 0, sizeof(t_str_ex));
	while (str[ex.i])
	{
		if (str[ex.i] == ENV_VAR)
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

int	str_expander_len_hd(char *str)
{
	t_str_ex	ex;

	memset(&ex, 0, sizeof(t_str_ex));
	while (str[ex.i])
	{
		if (str[ex.i] == ENV_VAR)
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

char	*str_expander_hd(char *str)
{
	char	*expanded;
	int		final_len;

	final_len = str_expander_len_hd(str);
	if (final_len <= 0)
		return (NULL);
	expanded = malloc(final_len + 1);
	if (!expanded)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	memset(expanded, 0, final_len + 1);
	expand_vars_hd(str, expanded, final_len);
	return (expanded);
}

char	*expand_input_hd(char *s)
{
	char	*expanded;

	expanded = str_expander_hd(s);
	if (!expanded)
	{
		free(s);
		return (ft_strdup(""));
	}
	free(s);
	s = expanded;
	return (s);
}
