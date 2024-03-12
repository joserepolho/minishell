/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:27:57 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/06 04:17:27 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execution(t_mini *mini, char **ev)
{
	t_command	*cmd;
	int			has_next;

	cmd = mini->commands;
	while (cmd)
	{
		if (!heredoc_signs_set(mini, cmd))
			return ;
		has_next = (cmd->next != NULL);
		if (has_next)
		{
			if (pipe(mini->input.pip) < 0)
				free_shell(PIPE_ERROR, EXIT_FAILURE, NULL, NULL);
		}
		if (cmd->args && !expand_command(cmd))
		{
			cmd = cmd->next;
			continue ;
		}
		mini->if_cd = 0;
		set_execution(mini, cmd, ev, has_next);
		cmd = cmd->next;
	}
	wait_for_children(mini);
}

void	wait_for_children(t_mini *mini)
{
	int			status;

	while (1)
	{
		waitpid(0, &status, 0);
		if (errno == ECHILD)
			break ;
		if (WIFEXITED(status))
			mini->command_ret = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			mini->command_ret = WTERMSIG(status);
	}
}

void	execute_in_parent(t_mini *mini, t_command *cmd, int has_next, int j)
{
	int	original_stdout;
	int	original_stdin;

	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	if (has_next)
		dup2(mini->input.pip[1], STDOUT_FILENO);
	setup_redirections(cmd, true);
	built_in(mini, cmd, j);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

void	pid_zero(t_command *cmd, char **ev, int has_next)
{
	if (mini()->input.cmd_input != STDIN_FILENO)
	{
		dup2(mini()->input.cmd_input, STDIN_FILENO);
		close(mini()->input.cmd_input);
	}
	if (has_next)
	{
		close(mini()->input.pip[0]);
		dup2(mini()->input.pip[1], STDOUT_FILENO);
		close(mini()->input.pip[1]);
	}
	setup_redirections(cmd, false);
	if (cmd->cmd_name != NULL && ft_strlen(cmd->cmd_name) > 0)
		if (!execution(cmd, ev))
			free_shell(NULL, 1, NULL, NULL);
	if (cmd->redirs && cmd->redirs->type == RED_AIN)
		dup2(mini()->input.pip[0], STDIN_FILENO);
	free_shell(NULL, EXIT_SUCCESS, NULL, NULL);
}

void	execute_in_child(t_command *cmd, char **ev, int has_next)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		pid_zero(cmd, ev, has_next);
	else if (pid < 0)
		free_shell(FORK_ERROR, EXIT_FAILURE, NULL, NULL);
	else
	{
		if (has_next)
			close(mini()->input.pip[1]);
		if (mini()->input.cmd_input != STDIN_FILENO)
			close(mini()->input.cmd_input);
		if (!has_next)
			waitpid(pid, &mini()->command_ret, 0);
		if (WIFEXITED(mini()->command_ret))
			mini()->command_ret = WEXITSTATUS(mini()->command_ret);
		else if (WIFSIGNALED(mini()->command_ret))
			mini()->command_ret = WTERMSIG(mini()->command_ret);
	}
}
