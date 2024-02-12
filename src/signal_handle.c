/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/10 02:42:06 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/10 03:10:51 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    prmpt_ctrlc(int signal)
{
    (void)signal;
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();   
}

void    sig_init(void)
{
	signal(SIGINT, prmpt_ctrlc);
	signal(SIGQUIT, SIG_IGN);
}
