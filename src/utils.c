/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:13:51 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/14 19:58:10 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum e_redir_type	redir_type(char *line)
{
	if (strncmp(line, REDIR_APPEND_IN, 2) == 0)
		return (RED_AIN);
	if (strncmp(line, REDIR_APPEND_OUT, 2) == 0)
		return (RED_AOUT);
	if (*line == REDIR_IN)
		return (RED_IN);
	if (*line == REDIR_OUT)
		return (RED_OUT);
	return (RED_NULL);
}

void	t_redir_init(t_redir *redir)
{
	redir->file = NULL;
	redir->fd = -1;
	redir->type = RED_NULL;
}

void	print_command(t_command *command)
{
	size_t	i;

	i = 0;
	printf("cmd_name: |%s|\n", command->cmd_name);
	printf("args: ");
	while (command->args[i])
	{
		printf("%s ", command->args[i]);
		i++;
	}
	printf("\n");
	if (command->in.file)
		printf("redir in type:|%d|file:|%s|\n", command->in.type,
			command->in.file);
	if (command->out.file)
		printf("redir out type:|%d|file:|%s|\n",
			command->out.type, command->out.file);
}
