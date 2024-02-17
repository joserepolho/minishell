/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:45 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/14 16:23:51 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* TODO: check for all types of semantic errors */
bool	input_error_check(t_mini *mini)
{
	char	*line_cursor;
	bool	open_quotes;
	bool	open_dquotes;

	line_cursor = mini->input.raw_line;
	open_quotes = false;
	open_dquotes = false;
	while (*line_cursor)
	{
		if (!skip_spaces(&line_cursor))
			return (false);
		if (*line_cursor == QUOTE)
			open_quotes = !open_quotes;
		else if (*line_cursor == DQUOTE)
			open_dquotes = !open_dquotes;
		else if (*line_cursor == PIPE)
			mini->input.pipe_c++;
		line_cursor++;
	}
	mini->input.len = line_cursor - mini->input.raw_line;
	return (!(open_quotes || open_dquotes)
		&& ft_strlen(mini->input.raw_line) > 0);
}

bool	skip_spaces(char **line)
{
	while (**line == ' ')
		(*line)++;
	return (**line);
}
