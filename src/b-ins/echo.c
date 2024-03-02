/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2024/02/14 01:21:08 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/14 01:21:08 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	bi_echo(char **av)
{
	bool	flag_n;
	int		i;

	i = 1;
	flag_n = false;
	if (av[i])
	{
		while (av[i] && !ft_strncmp(av[i], ECHO_FLAG_N, 2))
		{
			flag_n = true;
			i++;
		}
		while (av[i])
		{
			ft_putstr_fd(av[i], STDOUT_FILENO);
			if (av[i + 1])
				ft_putstr_fd(" ", STDOUT_FILENO);
			i++;
		}
	}
	if (!flag_n)
		ft_putstr_fd("\n", STDOUT_FILENO);
	mini()->command_ret = 0;
}
