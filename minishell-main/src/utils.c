/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:13:51 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/07 18:50:17 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum e_redir_type	redir_type(char *line)
{
	if (line == NULL)
		return (RED_NULL);
	if (strncmp(line, REDIR_APPEND_IN, 2) == 0)
		return (RED_AIN);
	if (strncmp(line, REDIR_APPEND_OUT, 2) == 0)
		return (RED_AOUT);
	if (*line == REDIR_IN)
		return (RED_IN);
	if (*line == REDIR_OUT)
		return (RED_OUT);
	return (RED_NULL);
}

bool	valid_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

bool	valid_cmd_arg(char *str)
{
	bool	quoted;
	int		l;

	quoted = has_char_in_set(str, "\"\'");
	if (!ft_strncmp(str, "$vari", 6) && mini()->if_cd)
		quoted = true;
	l = str_expander_len(str);
	if (!quoted && l == 0)
		return (false);
	return (l > 0 || quoted);
}

void	free_assign_null(void **ptr)
{
	if (*ptr)
		free(*ptr);
	*ptr = NULL;
}
