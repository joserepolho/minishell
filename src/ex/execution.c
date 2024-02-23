/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:27:57 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/23 04:59:16 by joaoribe         ###   ########.fr       */
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
	t_command	*cmd;
	int			has_next;

	cmd = mini->commands;
	while (cmd)
	{
		has_next = (cmd->next != NULL);
		if (has_next)
		{
			if (pipe(mini->input.pip) < 0)
				free_shell(PIPE_ERROR, EXIT_FAILURE, NULL, NULL);
		}
		handle_command(mini, cmd, ev, has_next);
		if (has_next)
		{
			// Update cmd_input for the next command to read from this pipe
			close(mini->input.pip[1]);
			// Close the write-end as it's not needed anymore
			mini->input.cmd_input = mini->input.pip[0];
		}
		cmd = cmd->next;
	}
}

void	handle_command(t_mini *mini, t_command *cmd, char **ev, int has_next)
{
	if (!has_next && cmd->in.type == -1 && cmd->out.type == -1
		&& if_builtin(cmd->cmd_name))
	{
		// Direct execution without forking if it's the last command with no redirections
		execute_direct(mini, cmd, ev);
	}
	else
	{
		// Setup and execute command in a child process
		execute_in_child(mini, cmd, ev, has_next);
	}
}

void	execute_direct(t_mini *mini, t_command *cmd, char **ev)
{
	// Direct execution logic (without forking)
	// This is a placeholder; actual execution logic depends on your shell's capabilities
	execution(mini, cmd, ev);
}

void	execute_in_child(t_mini *mini, t_command *cmd, char **ev, int has_next)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		// Child process
		if (mini->input.cmd_input != STDIN_FILENO)
		{
			dup2(mini->input.cmd_input, STDIN_FILENO);
			// Set up input redirection from previous pipe if necessary
			close(mini->input.cmd_input);
		}
		if (has_next)
		{
			close(mini->input.pip[0]);
			// Close read-end; not needed in child
			dup2(mini->input.pip[1], STDOUT_FILENO);
			close(mini->input.pip[1]);
			// Redirect stdout to pipe's write-end
		}
		setup_redirections(mini, cmd);  // Handle file-based redirections
		printf("1a");
		execution(mini, cmd, ev); // Execute the command
		free_shell(NULL, EXIT_SUCCESS, NULL, NULL);
	}
	else if (pid < 0)
		free_shell(FORK_ERROR, EXIT_FAILURE, NULL, NULL);
	else
	{
		// Parent process
		waitpid(pid, &mini->command_ret, 0);
		if (has_next)
			close(mini->input.pip[1]);
		// Close the write-end after child execution
		if (mini->input.cmd_input != STDIN_FILENO)
			close(mini->input.cmd_input);
		// Close the read-end of the previous pipe if it was used
		if (WIFEXITED(mini->command_ret))
		{
			#ifdef DEBUG
			printf("Child exited with status %d\n",
				WEXITSTATUS(mini->command_ret));
			#endif
			mini->command_ret = WEXITSTATUS(mini->command_ret);
				// Store child's exit status if needed
		}
		else if (WIFSIGNALED(mini->command_ret))
		{
			#ifdef DEBUG
			printf("Child terminated by signal %d\n",
				WTERMSIG(mini->command_ret));
			#endif
		}
	}
}

void	setup_redirections(t_mini *mini, t_command *cmd)
{
	int		fd_in;
	int		fd_out;
	char	*heredoc_fd;

	heredoc_fd = NULL;
	if (cmd->in.type != RED_NULL)
	{
		if (cmd->out.type == RED_IN)
			fd_in = open(cmd->in.file, O_RDONLY);
		else
		{
			heredoc_fd = heredoc(mini);
			fd_in = open(heredoc_fd, O_RDONLY);
			free(mini->hd_limiter);
			free(heredoc_fd);
		}
		if (fd_in < 0)
		{
			// perror("Error opening input file");
			error_msg(FD_NOT_FOUND, cmd->in.file);
			free_shell(NULL, 0, NULL, NULL);
		}
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
	}
	if (cmd->out.type != RED_NULL)
	{
		if (cmd->out.type == RED_OUT)
			fd_out = open(cmd->out.file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else
			fd_out = open(cmd->out.file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd_out < 0)
		{
			error_msg(FD_NOT_FOUND, cmd->out.file);
			free_shell(NULL, 0, NULL, NULL);
		}
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
}
