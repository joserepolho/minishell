/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:44:32 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/23 06:00:26 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_mini(t_mini *mini)
{
	mini->command_ret = 0;
	reset_mini(mini);
	mini->output = NULL;
	mini->hd_limiter = NULL;
}

t_mini	*mini(void)
{
	static t_mini	mini;

	return (&mini);
}

void	reset_mini(t_mini *mini)
{
	#ifdef DEBUG
		printf("resetting mini\n");
	#endif
	if (mini->input.raw_line)
	{
		free(mini->input.raw_line);
		mini->input.raw_line = NULL;
	}
	if (mini->input.cmd_input != STDIN_FILENO)
	{
		close(mini->input.cmd_input);
		mini->input.cmd_input = STDIN_FILENO;
	}
	if (mini->output)
		free(mini->output);
	mini->input.raw_line = NULL;
	mini->input.len = 0;
	mini->input.pipe_c = 0;
	if (mini->commands)
		free_commands(mini->commands);
	mini->commands = NULL;
}

t_list	*set_env(char **env)
{
	int		i;
	t_list	*env_list;
	t_list	*new_node;

	env_list = NULL;
	i = -1;
	while (env[++i])
	{
		new_node = ft_lstnew(ft_strdup(env[i]));
		if (!new_node)
		{
			ft_lstclear(&env_list, free);
			return (NULL);
		}
		ft_lstadd_back(&env_list, new_node);
	}
	return (env_list);
}
