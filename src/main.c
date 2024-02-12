/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:37 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/10 04:59:24 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	//char	**s;

	(void)av;
	(void)ac;
	(void)env;
	/* TODO: Load env? */
	init_mini(mini());
	while (1)
	{
		sig_init(); //CTRLC e CTRL/ durante prompt;
		mini()->input.raw_line = get_input(true);
		if (!input_error_check(mini()))
		{
			mini()->command_ret = false;
			printf("minishell: syntax error\n");
		}
		else
		{
			// size_t i = 0;
			parse(mini()->input.raw_line);
			/*while (s[i])
			{
				printf("s[%zu]: |%s|\n", i, s[i]);
				i++;
			}*/
			// free(mini()->input.raw_line);
			mini()->input.raw_line = NULL;
			// parse_input(mini());
		}
		// process line
		// free(mini()->input.raw_line);
		/* TODO: maybe create a mini.input free function */
	}
	free_shell(mini(), "Exit successful.\n", 0);
	return (0);
}
