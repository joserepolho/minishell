/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 03:56:01 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/12 05:43:56 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	if_builtin(char *s)
{
	if (!ft_strncmp(s, "cd", 2) || !ft_strncmp(s, "echo", 4)
		|| !ft_strncmp(s, "env", 3) || !ft_strncmp(s, "exit", 4)
		|| !ft_strncmp(s, "export", 6) || !ft_strncmp(s, "pwd", 3)
		|| !ft_strncmp(s, "unset", 5))
		return (1);
	return (0);
}

void	built_in(t_mini *mini, char **args)
{
	if (!ft_strncmp(args[0], "cd", 2))
		bi_cd(mini, args);
	else if (!ft_strncmp(args[0], "echo", 4))
		bi_echo(mini, args);
	else if (!ft_strncmp(args[0], "env", 3))
		bi_env(mini, args);
	else if (!ft_strncmp(args[0], "exit", 4))
		bi_exit(mini, args);
	else if (!ft_strncmp(args[0], "export", 6))
		bi_export(mini, args);
	else if (!ft_strncmp(args[0], "pwd", 3))
		bi_pwd(mini, args);
	else if (!ft_strncmp(args[0], "unset", 5))
		bi_unset(mini, args);
}
