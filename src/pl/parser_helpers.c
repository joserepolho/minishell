/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:09:31 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/22 18:26:44 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	redir_size(char *line)
{
	enum e_redir_type	type;

	type = redir_type(line);
	if (type == RED_AIN || type == RED_AOUT)
		return (2);
	if (type == RED_IN || type == RED_OUT || (line != NULL && (*line == '|'
				|| *line == ' ')))
		return (1);
	return (0);
}

void	assign_redir(t_command *command, char *redir_file,
		enum e_redir_type type)
{
	if (type == RED_IN || type == RED_AIN)
	{
		command->in.file = ft_strdup(redir_file);
		if (!command->in.file)
			free_shell(MALLOC_ERROR, STDERR_FILENO, free_commands_wrapper,
				command);
		command->in.type = type;
	}
	else
	{
		command->out.file = ft_strdup(redir_file);
		if (!command->out.file)
			free_shell(MALLOC_ERROR, STDERR_FILENO, free_commands_wrapper,
				command);
		command->out.type = type;
	}
}

void	assign_args(t_command *command, char **raw_commands, size_t end)
{
	size_t	i;
	size_t	k;
	char	**args;

	i = 0;
	while (i < end && redir_type(raw_commands[i]) == RED_NULL)
		i++;
	args = malloc((i + 1) * sizeof(char *));
	if (!args)
		free_shell(MALLOC_ERROR, STDERR_FILENO, free_commands_wrapper, command);
	args[i] = NULL;
	k = 0;
	while (k < i)
	{
		/* TODO: parse raw_command[k] before duping */
		args[k] = ft_strdup(raw_commands[k]);
		if (args[k] == NULL)
			free_shell(MALLOC_ERROR, STDERR_FILENO, free_commands_wrapper,
				command);
		k++;
	}
	command->args = args;
}

void	command_add_back(t_command *new_command)
{
	t_mini		*mi;
	t_command	*last_command;

	mi = mini();
	if (mi->commands == NULL)
	{
		mi->commands = new_command;
		return ;
	}
	last_command = mi->commands;
	while (last_command->next != NULL)
	{
		last_command = last_command->next;
	}
	last_command->next = new_command;
}
