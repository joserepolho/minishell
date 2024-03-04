/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:37 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/03 01:40:01 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	(void)av;
	(void)ac;
	memset(mini(), 0, sizeof(t_mini));
	mini()->env_list = set_env(env);
	rl_catch_signals = 0;
	if (!(mini()->env_list))
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
	while (1)
	{
		sig_init();
		mini()->input.raw_line = get_input(true);
		if (ft_strlen(mini()->input.raw_line) == 0)
		{
			free(mini()->input.raw_line);
			continue ;
		}
		add_history(mini()->input.raw_line);
		if (input_error_check(mini()) && parse_input(mini()))
			ft_execution(mini(), env);
		reset_mini(mini());
	}
	free_shell(NULL, 0, NULL, NULL);
}
