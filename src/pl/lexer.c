/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:45 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/10 04:27:45 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	input_error_check(t_mini *mini)
{
	/* TODO: check for all types of semantic errors */
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
	return (!(open_quotes || open_dquotes));
}

bool	skip_spaces(char **line)
{
	while (**line == ' ')
		(*line)++;
	return (*line);
}
