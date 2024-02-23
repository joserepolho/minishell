/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:29:00 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/23 04:35:39 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*heredoc(t_mini *mini)
{
	int				fd;
	char			*file;
	char			*input;
	struct termios	termios;

	input = NULL;
	file = ft_strdup("/tmp/hd");
	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		error_msg(FD_NOT_FOUND, "heredoc");
		free_shell(NULL, 0, NULL, NULL);
	}
	while (1)
	{
		tcgetattr(STDIN_FILENO, &termios);
		termios.c_cc[VQUIT] = _POSIX_VDISABLE;
		tcsetattr(STDIN_FILENO, TCSANOW, &termios); // change stdin to ignore SIGQUIT
		input = readline("> ");
		if (!ft_strncmp(input, mini->hd_limiter, ft_strlen(input)))
			break ;
		//tratar casos com $
		ft_putendl_fd(input, fd);
		free(input);
	}
	close(fd);
	return (file);
}
