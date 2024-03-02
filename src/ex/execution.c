/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:27:57 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/02 06:38:19 by joaoribe         ###   ########.fr       */
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
	int			i;
	t_command	*cmd;
	t_command	*lst;
	int			has_next;
	char		*heredoc_fd;

	cmd = mini->commands;
	lst = ft_lstlast_mini(cmd);
	while (cmd)
	{
		if (cmd->redirs && cmd->redirs->type == RED_AIN)
		{
			heredoc_fd = heredoc(mini);
			mini->hdfd = open(heredoc_fd, O_RDONLY);
			free(mini->hd_limiter);
			free(heredoc_fd);
		}
		has_next = (cmd->next != NULL);
		if (has_next)
		{
			if (pipe(mini->input.pip) < 0)
				free_shell(PIPE_ERROR, EXIT_FAILURE, NULL, NULL);
		}
		if (!expand_command(cmd, ev))
		{
			cmd = cmd->next;
			continue ;
		}
		if (has_next && if_builtin(lst->cmd_name) && !cmd->redirs)
			cmd = cmd->next;
		else
		{
		i = if_builtin(cmd->cmd_name);
		if (i)
			execute_in_parent(mini, cmd, has_next);
		else if (!i)
			execute_in_child(cmd, ev, has_next);
		if (has_next)
		{
			close(mini->input.pip[1]);
			mini->input.cmd_input = mini->input.pip[0];
		}
		cmd = cmd->next;
		}
	}
	wait_for_children(mini, i);
}

void	wait_for_children(t_mini *mini, int i)
{
	t_command	*cmd;

	cmd = mini->commands;
	while (cmd)
	{
		if (!i)
		{
			waitpid(0, &cmd->status, 0);
			if (WIFEXITED(cmd->status))
				mini->command_ret = WEXITSTATUS(cmd->status);
		}
		cmd = cmd->next;
	}
}

void	execute_in_parent(t_mini *mini, t_command *cmd, int has_next)
{
	int	original_stdout;
	int	original_stdin;

	original_stdout = dup(STDOUT_FILENO);
	original_stdin = dup(STDIN_FILENO);
	if (has_next)
		dup2(mini->input.pip[1], STDOUT_FILENO);
	setup_redirections(cmd, true);
	built_in(mini, cmd);
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

void	execute_in_child(t_command *cmd, char **ev, int has_next)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
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
		if (cmd->cmd_name != NULL)
			if (!execution(cmd, ev))
				free_shell(NULL, 1, NULL, NULL);
		if (cmd->redirs && cmd->redirs->type == RED_AIN)
			dup2(mini()->input.pip[0], STDIN_FILENO);
		free_shell(NULL, EXIT_SUCCESS, NULL, NULL);
	}
	else if (pid < 0)
		free_shell(FORK_ERROR, EXIT_FAILURE, NULL, NULL);
	else
	{
		//DEBUG_MSG("command_ret: %d\n", mini->command_ret);
		if (has_next)
			close(mini()->input.pip[1]);
		if (mini()->input.cmd_input != STDIN_FILENO)
			close(mini()->input.cmd_input);
		if (!has_next)
			waitpid(pid, &mini()->command_ret, 0);
		if (WIFEXITED(mini()->command_ret))
		{
			//DEBUG_MSG("Child terminated normally|%d|%d\n", mini->command_ret,
				WEXITSTATUS(mini()->command_ret);
			mini()->command_ret = WEXITSTATUS(mini()->command_ret);
		}
		else if (WIFSIGNALED(mini()->command_ret))
		{
			printf("Child terminated by signal %d\n",
				WTERMSIG(mini()->command_ret));
			/* need to handle signals */
		}
	}
}

void	setup_redirections(t_command *cmd, bool isparent)
{
	int		fd;
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir != NULL)
	{
		if (redir->type == RED_IN)
			fd = open(redir->file, O_RDONLY);
		else if (redir->type == RED_OUT)
			fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (redir->type == RED_AOUT)
			fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd < 0)
		{
			error_msg(FD_NOT_FOUND, redir->file);
			if (isparent)
				return ;
			free_shell(NULL, 0, NULL, NULL);
		}
		if ((redir->type == RED_IN))
			dup2(fd, STDIN_FILENO);
		else if ((redir->type == RED_AIN))
			dup2(mini()->hdfd, STDIN_FILENO);
		else if (redir->type == RED_OUT || redir->type == RED_AOUT)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
}
