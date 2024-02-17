/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 16:09:31 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/14 19:26:16 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	redir_size(char *line)
{
	enum e_redir_type	type;

	type = redir_type(line);
	if (type == RED_AIN || type == RED_AOUT)
		return (2);
	if (type == RED_IN || type == RED_OUT)
		return (1);
	return (0);
}

bool	should_split(char *line)
{
	return (redir_size(line) > 0 || *line == '|' || *line == ' ');
}

void	assign_redir(t_command *command, char *redir_file,
		enum e_redir_type type)
{
	if (type == RED_IN || type == RED_AIN)
	{
		command->in.file = ft_strdup(redir_file);
		command->in.type = type;
	}
	else
	{
		command->out.file = ft_strdup(redir_file);
		command->out.type = type;
	}
}

void	assign_args(t_command *command, char **raw_commands, size_t *i,
		size_t end)
{
	size_t	j;
	size_t	k;
	char	**args;

	j = 0;
	while (*i < end && redir_type(raw_commands[*i]) == RED_NULL)
	{
		(*i)++;
		j++;
	}
	args = malloc((j + 1) * sizeof(char *));
	if (!args)
		return ; /* TODO: malloc error */
	args[j] = NULL;
	k = 0;
	while (k < j)
	{
		args[k] = ft_strdup(raw_commands[*i - j + k]);
		k++;
	}
	command->args = args;
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
	while (last_command && last_command->next)
		last_command = last_command->next;
	last_command->next = new_command;
}
