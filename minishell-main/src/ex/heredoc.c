/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:29:00 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/08 00:18:21 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc_fd_set(char *file)
{
	int	fd;

	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		error_msg(FD_NOT_FOUND, "heredoc");
		free_shell(NULL, 0, NULL, NULL);
	}
	return (fd);
}

void	heredoc_pid_zero(t_mini *mini, char *delim, char *input, char *file)
{
	int	fd;

	fd = heredoc_fd_set(file);
	signal(SIGINT, SIG_DFL);
	while (1)
	{
		input = readline("> ");
		if (!input || (!ft_strncmp(input, delim,
					ft_strlen(input)) && input[0] != '\0'))
		{
			if (input)
				free(input);
			break ;
		}
		if (!mini->lim_q)
			input = expand_input_hd(input);
		ft_putendl_fd(input, fd);
		free(input);
	}
	close(fd);
	free(file);
	free_shell(NULL, 0, NULL, NULL);
}

void	heredoc_pid(t_mini *mini)
{
	waitpid(0, &mini->command_ret, 0);
	if (mini->command_ret == SIGINT)
		mini->command_ret = 130;
}

char	*heredoc(t_mini *mini, char *delim)
{
	pid_t			pid;
	char			*file;
	char			*input;
	struct termios	termios;
	struct termios	termios_backup;

	input = NULL;
	file = ft_strdup("/tmp/hd");
	tcgetattr(STDIN_FILENO, &termios_backup);
	termios = termios_backup;
	termios.c_cc[VQUIT] = _POSIX_VDISABLE;
	tcsetattr(STDIN_FILENO, TCSANOW, &termios);
	pid = fork();
	if (pid < 0)
		free_shell(FORK_ERROR, EXIT_FAILURE, NULL, NULL);
	if (pid == 0)
	{
		heredoc_pid_zero(mini, delim, input, file);
	}
	else
		heredoc_pid(mini);
	tcsetattr(STDIN_FILENO, TCSANOW, &termios_backup);
	return (file);
}
