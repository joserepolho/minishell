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

void    bi_echo(t_mini *mini, char **av)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
    if (!av[1])
        printf("\n");
    if (!ft_strncmp(av[1], "-n\0", 3))
	{
		flag = 1;
		i++;
	}
	while (av[i] && av[i][0])
	{
		printf("%s", av[i]);
		i++;
		if (av[i] && av[i][0])
			printf(" ");
	}
	if (!flag)
		printf("\n");
}