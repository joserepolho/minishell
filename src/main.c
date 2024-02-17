/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:37 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/17 21:56:42 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_command	*cmd;

	// char	**s;
	(void)av;
	(void)ac;
	(void)env;
	/* TODO: Load env? */
	cmd = NULL;
	init_mini(mini());
	mini()->env_list = set_env(env);
	if (!(mini()->env_list))
		free_shell(mini(), "Error\nMalloc failed!\n", "1");
	while (1)
	{
		mini()->input.raw_line = get_input(true);
		if (!input_error_check(mini()))
		{
			mini()->command_ret = false;
			printf("minishell: syntax error\n");
		}
		else
		{
			parse_input(mini());
			cmd = mini()->commands;
			while (cmd->next != NULL)
			{
				print_command(cmd->next);
				cmd = cmd->next;
				if (cmd->next != NULL)
					printf("will pipe to: \n");
			}
			ft_execution(mini(), env);
			free_mini(mini());
		}
	}
	free_mini(mini());
	return (0);
}
