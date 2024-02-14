/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:27:57 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/14 01:19:22 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void    ft_execution(t_mini *mini)
{
	pid_t		pid;
	t_command	*l_cmd;

	// sinal caso processo seja interrompido a meio
	pid = 0;
	l_cmd = malloc(sizeof(t_command));
	if (!l_cmd)
		free_shell(mini, "Error\nMalloc failure\n", 1);
	l_cmd = ft_lstlast(mini->commands);
	if (mini->commands->args && mini->commands->args[0])
	{
		if (!(mini->commands->next) && !ft_strncmp(mini->commands->args[0], "exit", 4))
			free_shell(mini, NULL, mini->commands->args[1]); // executar built-in direto se for so 1 comando exit
		else
		{
			while (mini->commands)
			{
				if (pipe(mini->input.pipe_c) < 0)
					free_shell(mini, "Error\nPipe failure!\n", 1);
				pid = fork();
				if (pid < 0)
					free_shell(mini, "Error\nFork failure!\n", 1);
				if (!pid)
					child_process(mini, mini->commands, mini->commands->args[0], l_cmd->cmd_name);
				else
					parent_process(mini, mini->commands, mini->commands->args[0], l_cmd->cmd_name);
				mini->commands = mini->commands->next;
			}
		}
	}
	else
		free_cmds(&mini->commands);
	free(l_cmd);
}

void	child_process(t_mini *mini, t_command *cmd, char *f_cmd, char *l_cmd)
{
	// sinal caso o pipe quebre
	close(mini->input.pipe_c[0]);
	if (!ft_strncmp(f_cmd, mini->commands->cmd_name, ft_strlen(f_cmd)))
	{
		dup2(mini->input.cmd_input, STDIN_FILENO);
		close(mini->input.cmd_input);
	}
	if (!ft_strncmp(f_cmd, l_cmd, ft_strlen(f_cmd)))
		dup2(mini->input.pipe_c[1], STDOUT_FILENO);
	close(mini->input.pipe_c[1]);
	execute(mini, cmd);
}

void	parent_process(t_mini *mini, t_command *cmd, char *f_cmd, char *l_cmd)
{
	if (ft_strncmp(f_cmd, mini->commands->cmd_name, ft_strlen(f_cmd)))
		close(mini->input.cmd_input);
	if (!ft_strncmp(f_cmd, l_cmd, ft_strlen(f_cmd)))
		mini->input.cmd_input = mini->input.pipe_c[0];
	else
		close(mini->input.pipe_c[0]);
	close(mini->input.pipe_c[1]);
	// sigint??
	wait(0);
}
