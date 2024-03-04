/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:29:00 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/04 01:59:42 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_vars_hd(char *str, char *expanded, int len)
{
	t_str_ex	ex;

	memset(&ex, 0, sizeof(t_str_ex));
	while (str[ex.i])
	{
		if (str[ex.i] == ENV_VAR)
		{
			ft_strlcat(expanded, str_expander_var_len(&ex, str), len + 1);
			free_assign_null((void **)&ex.var);
			continue ;
		}
		else
			ft_strlcat(expanded, str + ex.i, ft_strlen(expanded) + 2);
		ex.i++;
	}
	expanded[len] = '\0';
}

int	str_expander_len_hd(char *str)
{
	t_str_ex	ex;

	memset(&ex, 0, sizeof(t_str_ex));
	while (str[ex.i])
	{
		if (str[ex.i] == ENV_VAR)
		{
			str_expander_var_len(&ex, str);
			ex.len += ex.var_clen;
			free_assign_null((void **)&ex.var);
			continue ;
		}
		ex.len++;
		ex.i++;
	}
	return (ex.len);
}

char	*str_expander_hd(char *str)
{
	char	*expanded;
	int		final_len;

	final_len = str_expander_len_hd(str);
	if (final_len <= 0)
		return (NULL);
	expanded = malloc(final_len + 1);
	if (!expanded)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	memset(expanded, 0, final_len + 1);
	expand_vars_hd(str, expanded, final_len);
	return (expanded);
}

char	*expand_input_hd(char *s)
{
	char	*expanded;

	expanded = str_expander_hd(s);
	if (!expanded)
	{
		free(s);
		return (ft_strdup(""));
	}
	free(s);
	s = expanded;
	return (s);
}

char	*heredoc(t_mini *mini)
{
	int				fd;
	pid_t			pid;
	char			*file;
	char			*input;
	struct termios	termios;

	input = NULL;
	file = ft_strdup("/tmp/hd");
	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	pid = fork();
	if (fd < 0)
	{
		error_msg(FD_NOT_FOUND, "heredoc");
		free_shell(NULL, 0, NULL, NULL);
	}
	if (pid < 0)
		free_shell(FORK_ERROR, EXIT_FAILURE, NULL, NULL);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		while (1)
		{
			tcgetattr(STDIN_FILENO, &termios);
			termios.c_cc[VQUIT] = _POSIX_VDISABLE;
			tcsetattr(STDIN_FILENO, TCSANOW, &termios); // change stdin to ignore SIGQUIT
			input = readline("> ");
			if (!input || (!ft_strncmp(input, mini->hd_limiter, ft_strlen(input)) && input[0] != '\0'))
			{
				if (input)
					free(input);
				break ;
			}
			if (!mini->lim_q)
				input = expand_input_hd(input);
			ft_putendl_fd(input, fd);
			ft_putendl_fd(ft_itoa(mini->command_ret), fd);
			free(input);
		}
		close(fd);
	}
	else
	{
		waitpid(0, &mini->command_ret, 0);
		if (mini->command_ret == SIGINT)
			mini->command_ret = 130;
	}
	return (file);
}
