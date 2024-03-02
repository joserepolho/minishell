/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tiagoliv <tiagoliv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 20:13:48 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/29 14:10:49 by tiagoliv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	calculate_exit_code_from_string(const char *number)
{
	int		result;
	int		isnegative;
	size_t	str_size;
	size_t	i;

	result = 0;
	isnegative = number[0] == '-';
	i = isnegative;
	str_size = ft_strlen(number);
	while (i < str_size)
	{
		result = (result * 10 + (number[i] - '0')) % EXIT_STATUS_N;
		i++;
	}
	if (isnegative)
		result = EXIT_FAILURE - result;
	return (result);
}

bool	str_is_num(const char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	bi_exit(t_mini *mini, char **args, bool has_next)
{
	int		status;
	bool	valid;

	status = 0;

	if (ft_strlen(mini->commands->cmd_name) != 4)
		return (error_msg(CMD_NOT_FOUND, "exit"),
			mini->command_ret = 127, false);
	if (!args[1])
	{
		if (has_next)
			return (false);
		free_shell(NULL, mini->command_ret, NULL, NULL);
	}
	valid = str_is_num(args[1]);
	if (!valid)
		return (error_msg(EXIT_NUMERIR_ARG_REQ, args[1]),
			mini->command_ret = 1, false);
	else if (args[2])
		return (error_msg(TOO_MANY_ARGS, NULL), mini->command_ret = 1, false);
	status = calculate_exit_code_from_string(args[1]);
	if (has_next)
		return (mini->command_ret = status, false);
	free_shell(NULL, status, NULL, NULL);
	return (true);
}
