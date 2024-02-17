/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:27:57 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/17 21:54:11 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*ft_lstlast_mini(t_command *lst)
{
	if (!lst)
		return (0);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	ft_execution(t_mini *mini, char **ev)
{
	t_command	*tmp;
	pid_t		pid;
	char		*l_cmd;

	// sinal caso processo seja interrompido a meio
	pid = 0;
	tmp = mini->commands;
	l_cmd = ft_lstlast_mini(mini->commands)->cmd_name;
	if (mini->commands->cmd_name)
	{
		if (!(mini->commands->next))
		{
			execution(mini, tmp, ev);
		}
		else
		{
			while (tmp)
			{
				if (pipe(mini->input.pip) < 0)
					free_shell(mini, "Error\nPipe failure!\n", "1");
				pid = fork();
				if (pid < 0)
					free_shell(mini, "Error\nFork failure!\n", "1");
				if (!pid)
					child_process(mini, tmp, l_cmd, ev);
				else
					parent_process(mini, tmp, l_cmd);
				tmp = tmp->next;
			}
		}
	}
	// tratar sinais
}

void	child_process(t_mini *mini, t_command *cmd, char *l_cmd, char **ev)
{
	// sinal caso o pipe quebre
	close(mini->input.pip[0]);
	if (!ft_strncmp(mini->commands->cmd_name, cmd->cmd_name, ft_strlen(cmd->cmd_name)))
	{
		dup2(mini->input.cmd_input, STDIN_FILENO);
		close(mini->input.cmd_input);
	}
	if (!ft_strncmp(mini->commands->cmd_name, l_cmd, ft_strlen(cmd->cmd_name)))
		dup2(mini->input.pip[1], STDOUT_FILENO);
	if (cmd->in.type == RED_IN)
	{
		cmd->in.fd = open(cmd->in.file, O_RDONLY);
		if (!(cmd->in.fd))
			free_shell(mini, "Error\nFile open failed!\n", "1"); // checar o que deve acontecer se ficheiro nao existir
		dup2(cmd->in.fd, STDIN_FILENO);
		close(cmd->in.fd);
	}
	if (cmd->out.type == RED_OUT || cmd->out.type == RED_AOUT)
	{
		if (cmd->out.type == RED_OUT)
			cmd->out.fd = open(cmd->out.file, O_CREAT | O_RDWR | O_TRUNC);
		else if (cmd->out.type == RED_AOUT)
			cmd->out.fd = open(cmd->out.file, O_CREAT | O_RDWR | O_APPEND);
		if (!(cmd->in.fd))
			free_shell(mini, "Error\nFile open failed!\n", "1");
		dup2(cmd->in.fd, STDOUT_FILENO);
		close(cmd->in.fd);
	}
	close(mini->input.pip[1]);
	execution(mini, cmd, ev);
}

void	parent_process(t_mini *mini, t_command *cmd, char *l_cmd)
{
	wait(0);
	if (ft_strncmp(mini->commands->cmd_name, cmd->cmd_name, ft_strlen(cmd->cmd_name)))
		close(mini->input.cmd_input);
	if (!ft_strncmp(mini->commands->cmd_name, l_cmd, ft_strlen(cmd->cmd_name)))
		mini->input.cmd_input = mini->input.pip[0];
	else
		close(mini->input.pip[0]);
	close(mini->input.pip[1]);
	// sigint??
}
