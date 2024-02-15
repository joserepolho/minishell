/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 04:53:52 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/15 04:53:52 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_pwd(t_mini *mini)
{
	char	dir[PATH_MAX + 1];

	if (getcwd(dir, PATH_MAX))
		free_shell(mini, "Error\nFailure getting path!\n", 1);
	printf("%s\n", dir);
}
