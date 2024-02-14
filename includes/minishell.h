/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:27:57 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/14 04:41:39 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <signal.h>

# define PROMPT " > "
# define HEREDOC_PROMPT "heredoc>"

# define QUOTE '\''
# define DQUOTE '\"'
# define PIPE '|'

# define REDIR_IN '<'
# define REDIR_APPEND_IN "<<"
# define REDIR_OUT '>'
# define REDIR_APPEND_OUT ">>"

# define DEBUG

enum					e_redir_type
{
	RED_NULL = -1,
	RED_IN,
	RED_AIN,
	RED_OUT,
	RED_AOUT
};

typedef struct s_input
{
	char				*raw_line;
	int					cmd_input;
	size_t				len;
	size_t				pipe_c[2];
}						t_input;

typedef struct s_redir
{
	char				*file;
	enum e_redir_type	type;
}						t_redir;

typedef struct s_redirs
{
	t_redir				redirs;
	size_t				redir_c;
}						t_redirs;

typedef struct s_command
{
	char				*cmd_name;
	char				*raw_command;
	int					pipes; // poe nesta variavel o nr de pipes encontrados pfv
	int					*pipe_location; // poe nesta variavel a localizacao dos pipes na lista dos comandos. Da malloc consoante o nr de pipes
	char				**args;
	t_redirs			redirs;
	struct s_command	*next;
}						t_command;

typedef struct s_mini
{
	t_input				input;
	t_command			*commands;
	bool				command_ret;
	t_list				*env_list;
}						t_mini;

// main.c
int						main(int ac, char **av, char **env);

// free.c
void    				free_shell(t_mini *mini, char *err, int status);
void    				free_cmds(t_command **cmds);

// signal_handle.c
void    				sig_init(void);

// input.c
char					*get_input(bool prompt);
void					display_prompt(void);

// mini.c
void					init_mini(t_mini *mini);
t_mini					*mini(void);

// utils.c
bool					is_redir(char *line);
enum e_redir_type		redir_type(char *line);
bool					ft_isspace(char c);
char					*ft_strtrim2(char *s1, char const *set);
// pl
//  \ lexer.c
bool					input_error_check(t_mini *mini);
bool					skip_spaces(char **line);
//  \ parser.c
bool					parse_input(t_mini *mini);
void					command_add_back(t_command **command,
							t_command *new_command);
t_command				*parse_command(char *raw_command);
void					command_add_redir(t_command *command, char *redir);
char					**parse(char *line);
size_t					parse_size(char *line);
bool					should_split(char *line);
char					*get_next_section(char **line);
//  \ parser_helpers.c
size_t					redir_size(char *line);
size_t					count_args(char *line);
char					*get_next_arg(char **line);

// ex
// \ execution.c
void    				ft_execution(t_mini *mini);
void					child_process(t_mini *mini, t_command *cmd, char *first_cmd, char *lst_cmd);
void					parent_process(t_mini *mini, t_command *cmd, char *first_cmd, char *lst_cmd);
// \ execute.c
void					execution(t_mini *mini, t_command *cmd);
char					*get_path(char *cmd, char **ev);
char					*cmd_path(char **ev);
void					ft_free(char **s);

// b-ins
// \ built-ins_utils.c
int						if_builtin(char *s);
void					built_in(t_mini *mini, char **args);
// \ cd
// \ echo
void    bi_echo(t_mini *mini, char **av);
// \ env
// \ export
// \ pwd
// \ unset
#endif