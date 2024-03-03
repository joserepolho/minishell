/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 02:29:00 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/03 18:42:15 by tiagoliv         ###   ########.fr       */
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
	char			*file;
	char			*input;
	struct termios	termios;

	input = NULL;
	file = ft_strdup("/tmp/hd");
	if (!file)
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		error_msg(FD_NOT_FOUND, "heredoc");
		free_shell(NULL, 0, NULL, NULL);
	}
	mini->heredoc_is_running = true;
	signal(SIGINT, hd_ctrlc);
	while (1 && mini->heredoc_is_running)
	{
		tcgetattr(STDIN_FILENO, &termios);
		termios.c_cc[VQUIT] = _POSIX_VDISABLE;
		tcsetattr(STDIN_FILENO, TCSANOW, &termios); // change stdin to ignore SIGQUIT
		input = readline("> ");
		DEBUG_MSG("line:%s|\n", input);
		if (!mini->heredoc_is_running || !input || (!ft_strncmp(input, mini->hd_limiter, ft_strlen(input)) && input[0] != '\0'))
		{
			DEBUG_MSG("heredoc interrupted|%d|\n", mini->heredoc_is_running);
			if (!mini->heredoc_is_running)
			{
				DEBUG_MSG("heredoc interrupted by ctrl-c\n");
				free(file);
				return (NULL);
			}
			if (input)
				free(input);
			break ;
		}
		if (!mini->lim_q)
			input = expand_input_hd(input);
		ft_putendl_fd(input, fd);
		//ft_putendl_fd(ft_itoa(mini->command_ret), fd);/* isto é o que ??? */
		free(input);
	}
	mini->heredoc_is_running = false;
	close(fd);
	return (file);
}
