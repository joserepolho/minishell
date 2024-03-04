/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 02:42:06 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/04 01:26:48 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	prmpt_ctrlc(int signal)
{
	(void)signal;
	mini()->command_ret = 130;
	printf("^C\n");
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
	if (signal == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)", 2);
		mini()->command_ret = SIGQUIT;
	}
	else if (signal == SIGPIPE)
		mini()->command_ret = SIGPIPE;
	else if (signal == SIGINT)
		mini()->command_ret = 130;
}

void	hd_ctrlc(int sign)
{
	signal(sign, SIG_DFL);
}
