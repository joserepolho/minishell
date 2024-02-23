/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:28:37 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/23 05:05:01 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	(void)av;
	(void)ac;
	init_mini(mini());
	mini()->env_list = set_env(env);
	if (!(mini()->env_list))
		free_shell(MALLOC_ERROR, STDERR_FILENO, NULL, NULL);
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
			if (mini()->commands != NULL
				&& ft_strncmp(mini()->commands->cmd_name, "exit", 4) == 0)
			{
				if (mini()->commands->args && mini()->commands->args[0]
					&& mini()->commands->args[1] && mini()->commands->args[2])
					error_msg(TOO_MANY_ARGS, "exit");
				else
					break ;
			}
			ft_execution(mini(), env);
		}
		reset_mini(mini());
	}
	free_shell(NULL, 0, NULL, NULL);
}
