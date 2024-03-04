/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:27:57 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/04 02:00:09 by joaoribe         ###   ########.fr       */
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
	int			j;
	t_command	*cmd;
	t_command	*lst;
	int			has_next;
	char		*heredoc_fd;

	cmd = mini->commands;
	lst = ft_lstlast_mini(cmd);
	while (cmd)
	{
		signal(SIGQUIT, exec_sig);
		signal(SIGPIPE, exec_sig);
		signal(SIGINT, exec_sig);
		if (cmd->redirs && cmd->redirs->type == RED_AIN)
		{
			heredoc_fd = heredoc(mini);
			mini->hdfd = open(heredoc_fd, O_RDONLY);
			free(mini->hd_limiter);
			free(heredoc_fd);
			if (mini->command_ret == 130)
				return ;
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
		j = mini->input.pipe_c;
		i = if_builtin(cmd->cmd_name);
		if (j && (if_builtin_epe(cmd->cmd_name))
			&& cmd == lst && !cmd->redirs)
		{
			close(mini->input.pip[0]);
			built_in(mini, cmd, 0);
			cmd = cmd->next;
		}
		else if (j && i && cmd == lst && !cmd->redirs)
			cmd = cmd->next;
		else if (j || !j || cmd == lst || cmd != lst)
		{
			if (i)
				execute_in_parent(mini, cmd, has_next, j);
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
