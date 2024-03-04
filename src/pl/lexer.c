/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:45 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/02 06:01:28 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	input_error_check(t_mini *mini)
{
	char	*line_cursor;
	bool	quotes;
	bool	dquotes;

	line_cursor = mini->input.raw_line;
	quotes = false;
	dquotes = false;
	while (*line_cursor)
	{
		if (!skip_spaces(&line_cursor))
			return (false);
		if (*line_cursor == QUOTE && !dquotes)
			quotes = !quotes;
		else if (*line_cursor == DQUOTE && !quotes)
			dquotes = !dquotes;
		else if (*line_cursor == PIPE && !quotes && !dquotes)
			mini->input.pipe_c++;
		line_cursor++;
	}
	mini->input.len = line_cursor - mini->input.raw_line;
	if (quotes || dquotes)
		return (error_msg_ret(OPEN_QUOTES_ERROR, NULL, EXIT_FAILURE), false);
	return (true);
}

bool	skip_spaces(char **line)
{
	while (**line == ' ')
		(*line)++;
	return (**line);
}

// check for all types of semantic errors
bool	semantic_checker(char **sections)
{
	bool	isvalid;
	int		i;
	char	*error;
	char	*last_section;

	last_section = NULL;
	isvalid = true;
	i = 0;
	error = NULL;
	while (sections && sections[i] && isvalid && !error)
	{
		isvalid = valid_section(sections, &i, &last_section, &error);
		i++;
	}
	if (error)
		return (error_msg_ret(SYNTAX_ERROR, error, EXIT_FAILURE), false);
	return (isvalid);
}

bool	valid_section(char **sections, int *i,
	char **last_section, char **error)
{
	if (*sections[*i] == PIPE)
	{
		if (!*last_section)
			return (*error = sections[*i], false);
		else if (!sections[*i + 1])
			return (*error = "newline'", false);
	}
	if (redir_type(*last_section) != RED_NULL)
	{
		if (redir_type(sections[*i]) != RED_NULL || *sections[*i] == PIPE)
			return (*error = sections[*i], false);
		(*i)++;
	}
	else if (redir_type(sections[*i]) != RED_NULL)
	{
		if (!sections[*i + 1])
			return (*error = "newline'", false);
		else if (redir_type(sections[*i + 1]) != RED_NULL
			|| *sections[*i + 1] == PIPE)
			return (*error = sections[*i + 1], false);
		(*i)++;
	}
	*last_section = sections[*i];
	return (true);
}
