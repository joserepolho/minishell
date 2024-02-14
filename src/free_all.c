/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 23:03:13 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/14 01:12:13 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
// limpar comandos da memoria. usar este depois de uma execucao
void    free_cmds(t_command **cmds)
{
    int i;

    i = -1;
    while (*cmds)
    {
        if ((*cmds)->args)
        {
            while ((*cmds)->args[++i])
                free((*cmds)->args[i]);
            free((*cmds)->args);
        }
        if ((*cmds)->cmd_name)
            free((*cmds)->cmd_name);
        // if ((*cmds)->redirs->redirs)
            // free((*cmds)->redirs->redirs); se for para por pointer
        //limpar coisas a medida que for necessario
        (*cmds) = (*cmds)->next;
    }
    free(*cmds);
}
// limpar memoria e fechar shell
void    free_shell(t_mini *mini, char *err, int status)
{
	if (mini->input.raw_line)
		free(mini->input.raw_line);
	if (mini->commands)
        free_cmds(&mini->commands);
	//if (mini->commands->pipe)
//	{
		//close(mini->commands->pipe[0]);
		//close(mini->commands->pipe[1]);
	//}
	free(mini);
    // funcao para limpar envs do export
	rl_clear_history();
    if (err)
        write(2, err, ft_strlen(err));
    close(0);
    close(1);
    exit(status);
}
