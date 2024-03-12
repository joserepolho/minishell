/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:47 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/07 20:09:07 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_input(t_mini *mini)
{
	int			commands;
	size_t		i;
	size_t		j;
	char		**raw_commands;
	t_command	*command;

	raw_commands = parse(mini);
	if (!semantic_checker(raw_commands))
		return (free_list(raw_commands), false);
	commands = ((int)mini->input.pipe_c) + 1;
	if (mini->solo_pipe)
		commands--;
	i = 0;
	j = 0;
	while (commands > 0)
	{
		i = j;
		while (raw_commands[j] && *raw_commands[j] != PIPE)
			j++;
		command = construct_command(raw_commands + i, j - i);
		if (!command)
			return (free_list(raw_commands), false);
		command_add_back(command);
		commands--;
		j++;
	}
	return (free_list(raw_commands), true);
}

size_t	parse_size(char *line)
{
	int		si;
	char	*section;

	section = NULL;
	si = 0;
	while (*line)
	{
		skip_spaces(&line);
		section = get_next_section(&line);
		if (!section)
			return (0);
		free(section);
		si++;
	}
	return (si);
}

char	*get_next_section(char **line)
{
	bool	quotes;
	bool	dquotes;
	int		i;

	quotes = false;
	dquotes = false;
	i = 0;
	if (redir_size(*line) > 0)
		return (get_redir(line));
	while (**line && (quotes || dquotes || redir_size(*line) == 0))
	{
		if (**line == QUOTE)
			quotes = !quotes;
		else if (**line == DQUOTE)
			dquotes = !dquotes;
		i++;
		(*line)++;
	}
	return (ft_substr(*line - i, 0, i));
}

char	**parse(t_mini *mini)
{
	size_t	sections;
	size_t	i;
	char	*line;
	char	**s;

	line = mini->input.raw_line;
	sections = parse_size(line);
	i = 0;
	s = malloc((sections + 1) * sizeof(char *));
	if (!s)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	s[sections] = NULL;
	while (i < sections)
	{
		skip_spaces(&line);
		s[i] = get_next_section(&line);
		if (s[i] == NULL)
			return (free_list(s), NULL);
		i++;
	}
	return (s);
}

t_command	*construct_command(char **raw_commands, size_t end)
{
	t_command	*command;
	size_t		i;

	command = malloc(sizeof(t_command));
	if (!command)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	i = 0;
	ft_memset(command, 0, sizeof(t_command));
	while (i < end)
	{
		if (!update_command(command, raw_commands, &i, end))
			return (free(command), NULL);
		i++;
	}
	if (command->args == NULL)
		command->cmd_name = "";
	else
		command->cmd_name = command->args[0];
	command->next = NULL;
	return (command);
}
