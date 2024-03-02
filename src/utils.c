/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 17:13:51 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/27 19:28:06 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

enum e_redir_type	redir_type(char *line)
{
	if (line == NULL)
		return (RED_NULL);
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

bool	valid_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

bool	quoted_str(char *str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		if (*str == '\'' || *str == '\"')
			return (true);
		i++;
	}
	return (false);
}

bool	valid_cmd_arg(char *str)
{
	bool	quoted;
	int		l;

	quoted = quoted_str(str);
	l = str_expander_len(str);
	if (!quoted && l == 0)
		return (false);
	return (l > 0 || quoted);
}

void	free_assign_null(void **ptr)
{
	if (*ptr)
		free(*ptr);
	*ptr = NULL;
}

void	print_command(t_command *command)
{
	size_t		i;
	t_redir		*redir;
	t_command	*tmp;

	i = 0;
	if (ft_strlen(command->cmd_name) != 0)
		DEBUG_MSG("print_command: ");
	while (command->args && command->args[i])
	{
		printf("|%s| ", command->args[i]);
		i++;
	}
	if (ft_strlen(command->cmd_name) != 0)
		printf("\n");
	tmp = command;
	while (tmp)
	{
		redir = command->redirs;
		while (redir)
		{
			DEBUG_MSG("redir:|%s| type:", redir->file);
			if (redir->type)
			{
				if (redir->type == RED_IN)
					printf("RED_IN\n");
				else if (redir->type == RED_AIN)
					printf("RED_AIN\n");
				else if (redir->type == RED_OUT)
					printf("RED_OUT\n");
				else if (redir->type == RED_AOUT)
					printf("RED_AOUT\n");
			}
			redir = redir->next;
		}
		tmp = tmp->next;
		if (tmp)
			DEBUG_MSG("output will be piped to:\n");
	}
}
