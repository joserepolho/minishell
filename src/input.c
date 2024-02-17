/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:36:06 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/17 21:51:01 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_input(bool prompt)
{
	char	*line;
	char	*tmp;

	if (prompt)
		display_prompt();
		// return ("cat aaa> b$ABCbb < tt");
#ifdef DEBUG
	printf("-------------------------------------------\n");
#endif
	line = get_next_line(STDIN_FILENO);
#ifdef DEBUG
	printf("%s", line);
#endif
	if (!line)
	{
		ft_putstr_fd("exit\n", STDOUT_FILENO); /* TODO: what to do here? */
		exit(0);
	}
	tmp = ft_strtrim(line, " \n");
	free(line);
	return (tmp);
}

void	display_prompt(void)
{
	/* TODO: display last program success */
	/* TODO: display pwd */
	ft_putstr_fd(PROMPT, STDOUT_FILENO);
}
