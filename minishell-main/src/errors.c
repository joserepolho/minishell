/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:36:39 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/07 01:43:57 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_msg(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (!ft_strncmp(cmd, NOT_VALID_IDENT, ft_strlen(NOT_VALID_IDENT) + 1)
		|| !ft_strncmp(cmd, SYNTAX_ERROR, ft_strlen(SYNTAX_ERROR) + 1))
		ft_putstr_fd("`", STDERR_FILENO);
	if (msg)
		ft_putstr_fd(msg, STDERR_FILENO);
	if (!ft_strncmp(cmd, NOT_VALID_IDENT, ft_strlen(NOT_VALID_IDENT) + 1)
		|| !ft_strncmp(cmd, SYNTAX_ERROR, ft_strlen(SYNTAX_ERROR) + 1))
		ft_putstr_fd("'", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

void	error_msg_ret(char *cmd, char *msg, int ret)
{
	error_msg(cmd, msg);
	mini()->command_ret = ret;
}
