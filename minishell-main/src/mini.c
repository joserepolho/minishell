/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 14:44:32 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/07 02:28:35 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_mini	*mini(void)
{
	static t_mini	mini;

	return (&mini);
}

void	reset_mini(t_mini *mini)
{
	if (mini->input.raw_line)
		free(mini->input.raw_line);
	if (mini->input.cmd_input != STDIN_FILENO)
		close(mini->input.cmd_input);
	mini->input.cmd_input = STDIN_FILENO;
	mini->input.raw_line = NULL;
	mini->input.len = 0;
	mini->input.pipe_c = 0;
	mini->exit_unavailability = 0;
	mini->original_stdin_fd = 0;
	mini->home_tmp = NULL;
	mini->if_cd = 0;
	if (mini->commands && !mini->solo_pipe)
	{
		free_commands(mini->commands);
		mini->commands = NULL;
	}
	if (mini->output)
	{
		free(mini->output);
		mini->output = NULL;
	}
	if (!mini->solo_pipe)
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
