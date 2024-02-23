/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2024/02/15 04:53:52 by joaoribe          #+#    #+#             */
/*   Updated: 2024/02/15 04:53:52 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void	bi_pwd(void)
{
	char	dir[PATH_MAX + 1];

	if (!getcwd(dir, PATH_MAX))
		free_shell(FAILURE_GETTING_PATH, EXIT_FAILURE, NULL, NULL);
	printf("%s\n", dir);
}
