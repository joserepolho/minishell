/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:36:06 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/23 06:11:56 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_input(bool prompt)
{
	char	*line;

	if (prompt)
		display_prompt();
		// return ("cat aaa> b$ABCbb < tt");
#if defined(DEBUG)
	printf("-------------------------------------------\n");
#endif
	line = readline(mini()->output);
	if (*line && line)
		add_history(line);
#if defined(DEBUG) || defined(DEBUGG)
	printf("%s\n", line);
#endif
	if (!line)
	{
		ft_putendl_fd("!!!detected invalid input, will exit!", STDOUT_FILENO);
		line = ft_strdup("exit");
	}
	return (line);
}

void	display_prompt(void)
{
	char	dir[PATH_MAX + 1];
	char	*output;

	/* TODO: display last program success */
	// printf("cmd_ret: %d\n", mini()->command_ret);
	if (mini()->command_ret == 0)
		color_out(GREEN, CHECK " ");
	else
		color_out(RED, X);
	mini()->command_ret = 0;
	/* TODO: display pwd */
	if (getcwd(dir, PATH_MAX))
	{
		output = ft_strjoin(CYAN, dir);
		mini()->output = ft_strjoin(output, "$ \n");
		free(output);
	}
}
