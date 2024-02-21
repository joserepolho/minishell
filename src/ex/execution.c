/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/09 22:27:57 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/21 18:30:59 by tiagoliv         ###   ########.fr       */
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
			// Redirect stdout to pipe's write-end
		}
		setup_redirections(cmd);  // Handle file-based redirections
		execution(mini, cmd, ev); // Execute the command
		free_shell(NULL, EXIT_SUCCESS, NULL, NULL);
		exit(EXIT_SUCCESS);       // Ensure child process exits after execution
	}
	else if (pid < 0)
		free_shell(FORK_ERROR, EXIT_FAILURE, NULL, NULL);
	else
	{
		// Parent process
		wait(&mini->command_ret); // Wait for child to complete
		if (has_next)
			close(mini->input.pip[1]);// Close the write-end after child execution
		if (mini->input.cmd_input != STDIN_FILENO)
			close(mini->input.cmd_input); // Close the read-end of the previous pipe if it was used
	}
}

void	setup_redirections(t_command *cmd)
{
	int	fd_in;
	int	fd_out;

	if (cmd->in.type != RED_NULL)
	{
		fd_in = open(cmd->in.file, O_RDONLY);
		if (fd_in < 0)
		{
			// perror("Error opening input file");
			error_msg(FD_NOT_FOUND, cmd->in.file);
			exit(EXIT_FAILURE);
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
			// perror("Error opening output file");
			exit(EXIT_FAILURE);
		}
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
	}
}
