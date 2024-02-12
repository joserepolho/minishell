/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:47 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/11 05:45:09 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_input(t_mini *mini)
{
	size_t		commands;
	size_t		i;
	t_command	*command;
	char		**raw_commands;

	raw_commands = ft_split(mini->input.raw_line, PIPE);
	if (raw_commands == NULL || raw_commands[0] == NULL)
	{
		mini->commands = parse_c
#include "../../includes/push_swap.h"ommand(mini->input.raw_line);
		return ((bool)mini->commands);
	}
	commands = mini->input.pipe_c + 1;
	printf("commands: %zu\n", commands);
	i = 0;
	while (i < commands)
	{
		printf("raw_commands[%zu]: %s\n", i, raw_commands[i]);
		command = parse_command(raw_commands[i]);
		if (!command)
			return (false); /* TODO: free raw_comamnds */
		command_add_back(&mini->commands, command);
		i++;
	}
	return (true);
}

bool	should_split(char *line)
{
#include "../../includes/push_swap.h"
	return (redir_size(line) > 0 || *line == '|');
}

size_t	parse_size(char *line)
{
	int		si;
	char	*section;

	section = NULL;
	while (*line)
	{
		section = get_next_section(&line);
		printf("section: |%s|\n", section);
		if (section == NULL)
			break ;
		si++;
		line++;
	}
	printf("parse_size: %d\n", si + 1);
	return (si);
}

char	*get_next_section(char **line)
{
	char	*start;
	char	*section;
	bool	quotes;
	int		i;

	quotes = false;
	start = *line;
	while (*line)
	{
		if (**line == QUOTE || **line == DQUOTE)
			quotes = !quotes;
		if (!quotes && should_split(*line))
			break ;
		//(*line)++;
	}
	if (redir_size(*line) > 0 || **line == '|')
		i = redir_size(*line) || 1;
	section = ft_substr(start, 0, i);
	(*line) += i;
	return (section);
}

char	**parse(char *line)
{
	int		i;
	int		si;
	int		index;
	char	**s;
	bool	quotes;

	i = 0;
	si = 0;
	index = 0;
	quotes = false;
	if (ft_strlen(line) == 0)
		return (NULL);
	s = malloc((parse_size(line) + 1) * sizeof(char *));
	while (line[i])
	{
		if (line[i] == QUOTE || line[i] == DQUOTE)
			quotes = !quotes;
		else if (!quotes && should_split(line + i))
		{
			s[si] = ft_substr(line, index, i - index);
			printf("s[%d]: |%s|%ld\n", si, s[si], (line + i) - (line + index));
			si++;
			if (redir_size(line + i) > 0 || line[i] == '|')
			{
				s[si] = ft_substr(line, i, redir_size(line + i) || 1);
				printf("s[%d]: |%s|%ld|%d|%s\n", si, s[si], (line + i) - (line
						+ index), redir_size(line + i) || 1, line + i);
				i += redir_size(line + i);
				si++;
			}
			index = i;
		}
		i++;
	}
	s[si] = ft_substr(line, index, i - index);
	printf("s[%d]: |%s|%ld\n", si, s[si], (line + i) - (line + index));
	return (s);
}

char	**split_console_command(char *raw_command)
{
	char	**args;
	size_t	i;
	size_t	args_len;

	i = 0;
	args_len = count_args(raw_command);
	args = malloc(sizeof(char *) * (args_len + 1));
	printf("args_len: %zu\n", args_len);
	printf("raw_command: |%s|\n", raw_command);
	while (i < args_len)
	{
		skip_spaces(&raw_command);
		args[i] = get_next_arg(&raw_command);
		printf("args[%zu]: |%s|\n", i, args[i]);
		i++;
	}
	args[i] = NULL;
	return (args);
}

t_command	*parse_command(char *raw_command)
{
	t_command	*command;
	char		**args;

	command = malloc(sizeof(t_command));
	if (!command)
		return (NULL);
	command->raw_command = raw_command;
	args = split_console_command(raw_command);
	command->cmd_name = args[0];
	command->args = args;
	command->redirs.redir_c = 0;
	command->next = NULL;
	return (command);
}

void	command_add_back(t_command **command, t_command *new_command)
{
	t_command	*last_command;

	if (!*command)
	{
		*command = new_command;
		return ;
	}
	last_command = *command;
	while (last_command->next)
		last_command = last_command->next;
	last_command->next = new_command;
}

void	command_add_redir(t_command *command, char *redir)
{
	// t_redir *new_redir;
	(void)command;
	(void)redir;
	/*new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;

	new_redir->file = redir + 1;
	if (*redir == REDIR_IN)
		new_redir->type = REDIR_IN;
	else if (*redir == REDIR_APPEND_IN)
		new_redir->type = REDIR_APPEND_IN;
	else if (*redir == REDIR_OUT)
		new_redir->type = REDIR_OUT;
	else if (*redir == REDIR_APPEND_OUT)
		new_redir->type = REDIR_APPEND_OUT;*/
	/*new_redir->next = command->redirs;
	command->redirs = new_redir;*/
}