/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:47 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/22 20:16:37 by tiagoliv         ###   ########.fr       */
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
	if (!raw_commands)
		free_shell(MALLOC_ERROR, EXIT_FAILURE, free, raw_commands);
	if (!semantic_checker(raw_commands))
		return (free_list(raw_commands), false);
	commands = ((int)mini->input.pipe_c) + 1;
	i = 0;
	j = 0;
	while (commands > 0)
	{
		i = j;
		while (raw_commands[j] && *raw_commands[j] != PIPE)
			j++;
		command = construct_command(mini, raw_commands + i, j - i);
		if (!command)
			free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
		command_add_back(command);
		commands--;
		j++;
	}
	free_list(raw_commands);
	return (true);
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
	char	*start;
	bool	quotes;
	int		i;
	bool	escaped;

	quotes = false;
	start = *line;
	escaped = false;
	while (**line && (quotes || !redir_size(*line)) && skip_spaces(line))
	{
		if (**line == ESCAPE_CHAR || escaped)
			escaped = !escaped;
		else if (**line == QUOTE || **line == DQUOTE)
			quotes = !quotes;
		(*line)++;
	}
	i = *line - start;
	if (i == 0 && redir_size(*line))
	{
		i = redir_size(*line);
		(*line) += i;
	}
	return (substr_expander(start, i));
	/*return (ft_substr(start, 0, i));*/
}

char	**parse(t_mini *mini)
{
	size_t	sections;
	size_t	i;
	char	*line;
	char	**s;

	// char	*tmp;
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
		/*tmp = get_next_section(&line);
		if (!tmp)
			return (NULL);
		s[i] = ft_strtrim(tmp, " ");
		printf("section:|%s|\n", s[i]);
		free(tmp);*/
		s[i] = get_next_section(&line);
		if (s[i] == NULL)
			return (free_list(s), NULL);
		// tmp = NULL;
		i++;
	}
	return (s);
}

t_command	*construct_command(t_mini *mini, char **raw_commands, size_t end)
{
	t_command			*command;
	size_t				i;
	enum e_redir_type	type;

	command = malloc(sizeof(t_command));
	if (!command)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	i = 0;
	command->cmd_name = ft_strdup(raw_commands[i]);
	if (!command->cmd_name)
		free_shell(MALLOC_ERROR, STDERR_FILENO, free, command);
	assign_args(command, raw_commands, end);
	t_redir_init(command);
	while (i < end)
	{
		type = redir_type(raw_commands[i]);
		if (type != RED_NULL)
			assign_redir(command, raw_commands[++i], type);
		if (type == RED_AIN)
			mini->hd_limiter = ft_strdup(raw_commands[i]);
		i++;
	}
	command->next = NULL;
	return (command);
}
