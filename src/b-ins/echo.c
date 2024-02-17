/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 01:21:08 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/14 01:21:08 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_echo(char **av)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (!av[0])
	{
		printf("\n");
		return ;
	}
	if (!ft_strncmp(av[0], "-n", 2))
	{
		flag = 1;
		i++;
	}
	while (av[i])
	{
		printf("%s", av[i]);
		i++;
		if (av[i] && av[i][0])
			printf(" ");
	}
	if (!flag)
		printf("\n");
}
