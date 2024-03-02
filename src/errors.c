/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 16:36:39 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/28 16:17:40 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	error_msg(char *cmd, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	if (msg)
		ft_putendl_fd(msg, STDERR_FILENO);
}

void	error_msg_ret(char *cmd, char *msg, int ret)
{
	error_msg(cmd, msg);
	mini()->command_ret = ret;
}
