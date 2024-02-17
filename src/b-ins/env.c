/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 01:50:53 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/14 01:50:53 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bi_env(t_list *env_list)
{
	while (env_list)
	{
		printf("%s\n", (char *)env_list->content);
		env_list = env_list->next;
	}
}
