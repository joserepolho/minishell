/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 21:19:38 by joaoribe          #+#    #+#             */
/*   Updated: 2024/03/07 22:54:50 by tiagoliv         ###   ########.fr       */
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

int	heredoc_signs_set(t_mini *mini, t_command *cmd)
{
	char	*heredoc_fd;
	char	*tmp;

	signal(SIGQUIT, exec_sig);
	signal(SIGPIPE, exec_sig);
	signal(SIGINT, exec_sig);
	if (cmd->redirs && cmd->redirs->type == RED_AIN)
	{
		tmp = remove_quotes(cmd->redirs->file);
		free(cmd->redirs->file);
		cmd->redirs->file = tmp;
		heredoc_fd = heredoc(mini, cmd->redirs->file);
		free(cmd->redirs->file);
		cmd->redirs->file = heredoc_fd;
		if (mini->command_ret == 130)
		{
			printf("\n");
			return (0);
		}
	}
	return (1);
}

void	bin_epe(t_mini *mini, t_command *cmd)
{
	close(mini->input.pip[0]);
	built_in(mini, cmd, 0);
}

void	fd_error(t_redir *redir, bool isparent)
{
	error_msg(FD_NOT_FOUND, redir->file);
	if (isparent)
		return ;
	free_shell(NULL, 0, NULL, NULL);
}

void	setup_redirections(t_command *cmd, bool isparent)
{
	int		fd;
	t_redir	*redir;

	redir = cmd->redirs;
	fd = 0;
	while (redir != NULL)
	{
		if (redir->type == RED_IN)
			fd = open(redir->file, O_RDONLY);
		else if (redir->type == RED_AIN)
			fd = open(redir->file, O_RDONLY);
		else if (redir->type == RED_OUT)
			fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		else if (redir->type == RED_AOUT)
			fd = open(redir->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
		if (fd < 0)
			fd_error(redir, isparent);
		if (redir->type == RED_IN || redir->type == RED_AIN)
			dup2(fd, STDIN_FILENO);
		else if (redir->type == RED_OUT || redir->type == RED_AOUT)
			dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
}
