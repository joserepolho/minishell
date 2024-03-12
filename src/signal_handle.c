/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 02:42:06 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/07 23:46:05 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prmpt_ctrlc(int signal)
{
	mini()->command_ret = SIG_BASE_RET + signal;
	write(1, "^C\n", 3);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sig_init(void)
{
	signal(SIGINT, prmpt_ctrlc);
	signal(SIGQUIT, SIG_IGN);
}

void	exec_sig(int signal)
{
	write(1, "\n", 1);
	mini()->command_ret = signal + SIG_BASE_RET;
	if (signal == SIGQUIT)
		ft_putstr_fd("Quit (core dumped)", 2);
}

char	*if_relative_path(char *cmd, bool home_added)
{
	char	*c;

	if (home_added)
		free(cmd);
	c = get_relative_path(cmd);
	return (c);
}
