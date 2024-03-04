/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 20:13:48 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/02 23:31:22 by joaoribe         ###   ########.fr       */
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

	if (!args[1])
	{
		if (has_next)
			return (false);
		free_shell(NULL, mini->command_ret, NULL, NULL);
	}
	valid = str_is_num(args[1]);
	if (!valid)
	{
		mini->exit_unavailability = 1;
		return (error_msg(EXIT_NUMERIR_ARG_REQ, args[1]),
			mini->command_ret = 1, false);
	}
	else if (args[2])
	{
		mini->exit_unavailability = 1;
		return (error_msg(TOO_MANY_ARGS, NULL), mini->command_ret = 1, false);
	}
	status = calculate_exit_code_from_string(args[1]);
	if (has_next)
		return (mini->command_ret = status, false);
	free_shell(NULL, status, NULL, NULL);
	return (true);
}
