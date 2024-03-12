/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 18:05:11 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/07 21:41:55 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	skip_spaces(char **line)
{
	if (**line == '|')
	{
		error_msg(SYNTAX_ERROR, "|");
		return (NULL);
	}
	while (**line == ' ')
	{
		if (**line == '|')
		{
			error_msg(SYNTAX_ERROR, "|");
			return (NULL);
		}
		(*line)++;
	}
	return (**line);
}


size_t	redir_size(char *line)
{
	enum e_redir_type	type;

	type = redir_type(line);
	if (type == RED_AIN || type == RED_AOUT)
		return (2);
	if (type == RED_IN || type == RED_OUT || (line != NULL && (*line == PIPE
				|| *line == ' ')))
		return (1);
	return (0);
}

bool	has_char_in_set(char *s, char *set)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (ft_strchr(set, s[i]))
			return (true);
		i++;
	}
	return (false);
}

char	*remove_quotes(char *file)
{
	char	*r;
	bool	quotes;
	bool	dquotes;
	int		i;
	int		j;

	r = malloc(remove_quotes_new_len(file) + 1);
	if (!r)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, NULL, NULL);
	i = 0;
	j = 0;
	quotes = false;
	dquotes = false;
	while (file && file[i])
	{
		if (file[i] == QUOTE && !dquotes)
			quotes = !quotes;
		else if (file[i] == DQUOTE && !quotes)
			dquotes = !dquotes;
		else
			r[j++] = file[i];
		i++;
	}
	r[j] = '\0';
	return (r);
}

int	remove_quotes_new_len(char *file)
{
	bool	quotes;
	bool	dquotes;
	int		i;
	int		len;

	quotes = false;
	dquotes = false;
	i = 0;
	len = 0;
	while (file && file[i])
	{
		if (file[i] == QUOTE && !dquotes)
			quotes = !quotes;
		else if (file[i] == DQUOTE && !quotes)
			dquotes = !dquotes;
		else
			len++;
		i++;
	}
	return (len);
}
