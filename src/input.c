/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:36:06 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/03 04:49:56 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_input(bool prompt)
{
	char	*line;
	char	*tmp;

	if (prompt)
		update_prompt();
	line = readline(mini()->output);
	if (line && *line)
	{
		tmp = ft_strtrim(line, " \t\n");
		free(line);
		if (!tmp)
			free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
		line = tmp;
	}
	else if (!line)
	{
		printf("exit\n");
		line = ft_strdup("exit");
	}
	return (line);
}

void	update_prompt(void)
{
	char	dir[PATH_MAX + 1];
	char	*color;
	char	*chr;
	char	*tmp;

	color = RED;
	chr = X;
	if (mini()->output)
	{
		free(mini()->output);
		mini()->output = NULL;
	}
	if (mini()->command_ret == 0)
	{
		color = GREEN;
		chr = CHECK;
	}
	if (getcwd(dir, PATH_MAX))
	{
		tmp = ft_strnjoin(6, color, chr, CYAN " ", dir, RESET, PROMPT);
		mini()->output = tmp;
	}
}
