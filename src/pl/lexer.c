/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:45 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/22 17:26:46 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* TODO: check for all types of semantic errors */
bool	input_error_check(t_mini *mini)
{
	char	*line_cursor;
	bool	quotes;
	bool	dquotes;
	bool	escaped;

	line_cursor = mini->input.raw_line;
	quotes = false;
	dquotes = false;
	escaped = false;
	while (*line_cursor)
	{
		if (!skip_spaces(&line_cursor))
			return (false);
		if (*line_cursor == ESCAPE_CHAR || escaped)
			escaped = !escaped;
		else if (*line_cursor == QUOTE)
			quotes = !quotes;
		else if (*line_cursor == DQUOTE)
			dquotes = !dquotes;
		else if (*line_cursor == PIPE)
			mini->input.pipe_c++;
		line_cursor++;
	}
	mini->input.len = line_cursor - mini->input.raw_line;
	return (!(quotes || dquotes) && ft_strlen(mini->input.raw_line) > 0);
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
	bool	last_was_redir;
	bool	isvalid;
	int		i;
	char	*error;

	last_was_redir = false;
	isvalid = true;
	i = 0;
	error = NULL;
	while (sections && sections[i])
	{
		isvalid = valid_arg(sections, i, &last_was_redir, &error);
		if (!isvalid || error != NULL)
			break ;
		i++;
	}
	/*printf("section:|%s|isnull:%d|%d|%d\n", sections[i], sections[i] == NULL,
		isvalid, last_was_redir);*/
	if (sections[i] != NULL)
	{
		valid_arg(sections, i, &last_was_redir, &error);
		if (error != NULL)
		{
			if (*error == '\n')
				error = "newline'";
			// printf("semantic_checker output: %d\n", 0);
			return (error_msg(SYNTAX_ERROR, error), false);
		}
	}
	if (error != NULL)
	{
		// printf("semantic_checker output: %d\n", 0);
		return (error_msg(SYNTAX_ERROR, error), false);
	}
	if (isvalid && last_was_redir)
	{
		// printf("semantic_checker output: %d\n", isvalid && last_was_redir);
		return (error_msg(SYNTAX_ERROR, "newline'"), false);
	}
	return (isvalid && !last_was_redir);
}

bool	valid_arg(char **sections, int i, bool *last_was_redir, char **error)
{
	char	*tmp;

	if (redir_type(sections[i]) != RED_NULL || *sections[i] == PIPE)
	{
		if (*last_was_redir || i == 0)
			return (*error = sections[i], false);
		*last_was_redir = true;
	}
	else if (*last_was_redir)
	{
		*last_was_redir = false;
		tmp = ft_strtrim(sections[i], " \n\t");
		/*printf("tmp:|%s|isnull:%d|%d|%d\n", tmp, tmp == NULL, *tmp == '\0',
			ft_strlen(tmp) == 0);*/
		if (!tmp || !*tmp || ft_strlen(tmp) == 0)
			return (free(tmp), *error = sections[i], false);
		free(tmp);
	}
	return (true);
}

bool	valid_command_or_arg(char *section)
{
	char	*tmp;

	/*int		i;
	bool	valid;*/
	tmp = ft_strtrim(section, " \n\t");
	if (!tmp || !*tmp || ft_strlen(tmp) == 0)
		return (free(tmp), false);
	free(tmp);
	return (true);
}
