/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:27:57 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/17 21:21:31 by joaoribe         ###   ########.fr       */
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
# include <limits.h>
# include <linux/limits.h>
# include <sys/wait.h>

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
	size_t				len;
	size_t				pipe_c;
	int					pip[2];
	int					cmd_input;
}						t_input;

typedef struct s_redir
{
	char				*file;
	int					fd;
	enum e_redir_type	type;
}						t_redir;

typedef struct s_command
{
	char				*cmd_name;
	char				**args;
	t_redir				in;
	t_redir				out;
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

// input.c
char					*get_input(bool prompt);
void					display_prompt(void);

// mini.c
void					init_mini(t_mini *mini);
t_mini					*mini(void);
void					reset_mini(t_mini *mini);
t_list					*set_env(char **env);

// utils.c
enum e_redir_type		redir_type(char *line);
void					free_commands(t_command *commands);
void					t_redir_init(t_redir *redir);
void					print_command(t_command *command);
// pl
//  \ lexer.c
bool					input_error_check(t_mini *mini);
bool					skip_spaces(char **line);
//  \ parser.c
bool					parse_input(t_mini *mini);
size_t					parse_size(char *line);
char					*get_next_section(char **line);
char					**parse(t_mini *mini);
t_command				*construct_command(char **raw_commands, size_t end);
//  \ parser_helpers.c
size_t					redir_size(char *line);
bool					should_split(char *line);
void					command_add_back(t_command **command,
							t_command *new_command);
void					assign_redir(t_command *command, char *redir_file,
							enum e_redir_type type);
void					assign_args(t_command *command, char **raw_commands,
							size_t *i, size_t end);
// free.c
void					free_commands(t_command *commands);
void					free_list(char **list);
void					free_mini(t_mini *mini);
void					free_shell(t_mini *mini, char *err, char *status);
// signal_handle.c
void					prmpt_ctrlc(int signal);
void					sig_init(void);
// ex
// \ execution.c
void					ft_execution(t_mini *mini, char **ev);
void					child_process(t_mini *mini, t_command *cmd,
							char *lst_cmd, char **ev);
void					parent_process(t_mini *mini, t_command *cmd,
							char *lst_cmd);
// \ execute.c
void					execution(t_mini *mini, t_command *cmd, char **ev);
char					*get_path(char *cmd, char **ev);
char					*cmd_path(char **ev);
// b-ins
// \ built-ins_utils.c
int						if_builtin(char *s);
void					built_in(t_mini *mini, t_command *cmd);
// \ cd
void					bi_cd(t_mini *mini, char **av);
// \ echo
void					bi_echo(char **av);
// \ env
void					bi_env(t_list *env_list);
// \ export
int						ft_strlen_eq(char *s);
void					free_content(void *content);
void					delete_var(t_list **head, void *node_to_del);
void					bi_export(t_mini *mini, char **av);
// \ pwd
void					bi_pwd(t_mini *mini);
// \ unset
void					bi_unset(t_mini *mini, char **av);
#endif
