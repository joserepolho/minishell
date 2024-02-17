/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:47 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/14 19:55:46 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_input(t_mini *mini)
{
	size_t		commands;
	size_t		i;
	size_t		j;
	char		**raw_commands;
	t_command	*command;

	raw_commands = parse(mini);
	commands = mini->input.pipe_c + 1;
	i = 0;
	j = 0;
	while (commands--)
	{
		i = j;
		while (raw_commands[j] && *raw_commands[j] != PIPE)
			j++;
		command = construct_command(raw_commands + i, j - i);
		if (!command)
			return (false); /* TODO: free raw_comamnds */
		command_add_back(&mini->commands, command);
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

	quotes = false;
	start = *line;
	while (**line)
	{
		if (!quotes && should_split(*line))
			break ;
		if (**line == QUOTE || **line == DQUOTE)
			quotes = !quotes;
		(*line)++;
	}
	if (*line - start == 0 && should_split(*line))
	{
		i = redir_size(*line);
		if (i == 0)
			i = 1;
		(*line) += i;
	}
	else
		i = *line - start;
	return (ft_substr(start, 0, i));
}

char	**parse(t_mini *mini)
{
	size_t	sections;
	size_t	i;
	char	*line;
	char	*tmp;
	char	**s;

	line = mini->input.raw_line;
	sections = parse_size(line);
	i = 0;
	s = malloc((sections + 1) * sizeof(char *));
	s[sections] = NULL;
	while (i < sections)
	{
		skip_spaces(&line);
		tmp = get_next_section(&line);
		if (!tmp)
			return (NULL);
		s[i] = ft_strtrim(tmp, " ");
		free(tmp);
		if (s[i] == NULL)
			return (free_list(s), NULL);
		tmp = NULL;
		i++;
	}
	return (s);
}

t_command	*construct_command(char **raw_commands, size_t end)
{
	t_command			*command;
	size_t				i;
	enum e_redir_type	type;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	i = 0;
	command->cmd_name = ft_strdup(raw_commands[i++]);
	assign_args(command, raw_commands, &i, end);
	t_redir_init(&command->in);
	t_redir_init(&command->out);
	while (i < end)
	{
		type = redir_type(raw_commands[i]);
		if (type != RED_NULL)
			assign_redir(command, raw_commands[++i], type);
		i++;
	}
	command->next = NULL;
	return (command);
}
