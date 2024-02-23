/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:27:57 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/02/23 05:54:44 by joaoribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <limits.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/wait.h>
# include <termios.h>

# define PROMPT "$ "
# define HEREDOC_PROMPT "heredoc>"

# define QUOTE '\''
# define DQUOTE '\"'
# define PIPE '|'

# define REDIR_IN '<'
# define REDIR_APPEND_IN "<<"
# define REDIR_OUT '>'
# define REDIR_APPEND_OUT ">>"

# define ESCAPE_CHAR '\\'
# define ENV_VAR '$'
# define SPACE ' '

# define MALLOC_ERROR "Malloc failed!\n"

# define CHECK "\xE2\x9C\x93"
# define X "\xE2\x9C\x97"

# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define BLUE "\033[0;34m"
# define CYAN "\033[0;36m"
# define RESET "\033[0m"

// error_msg errors
# define SHELL_ERROR "minishell: %s %s\n"
# define CMD_NOT_FOUND "command not found: "
# define FD_NOT_FOUND "no such file or directory: "
# define SYNTAX_ERROR "syntax error near unexpected token `"
# define TOO_MANY_ARGS "too many arguments: "
# define NOT_VALID_IDENT "not a valid identifier: "

// free shell errors
# define FAILURE_GETTING_PATH "Failure getting path!\n"
# define DIR_CHANGE_ERROR "Directory change failure!\n"
# define PIPE_ERROR "Pipe error!\n"
# define FORK_ERROR "Fork error!\n"

# define DEBUGG

enum					e_redir_type
{
	RED_NULL = -1,
	RED_IN,
	RED_AIN,
	RED_OUT,
	RED_AOUT
};

typedef void			(*t_cleanup_func)(void *);

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
	int					command_ret;
	t_list				*env_list;
	char				*hd_limiter;
	char				*output;
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
void					t_redir_init(t_command *command);
void					print_command(t_command *command);
// pl
//  \ lexer.c
bool					input_error_check(t_mini *mini);
bool					skip_spaces(char **line);
bool					semantic_checker(char **raw_commands);
bool					valid_arg(char **sections, int i,
							bool *last_was_redir, char **error);
bool					valid_command_or_arg(char *section);
//  \ parser.c
bool					parse_input(t_mini *mini);
size_t					parse_size(char *line);
char					*get_next_section(char **line);
char					**parse(t_mini *mini);
t_command				*construct_command(t_mini *mini,
							char **raw_commands, size_t end);
//  \ parser_helpers.c
size_t					redir_size(char *line);
void					command_add_back(t_command *new_command);
void					assign_redir(t_command *command, char *redir_file,
							enum e_redir_type type);
void					assign_args(t_command *command, char **raw_commands,
							size_t end);
//  \ parser_helpers2.c
char					*substr_expander(char *str, size_t len);
char					*replace_vars(char *str);
char					*replace_var(char **str, int *i);
/*char					*remove_layer_quotes(char *str);*/
bool					valid_env_char(char c);
// free.c
void					free_commands(t_command *commands);
void					free_list(char **list);

void					free_shell(char *err, int status,
							void (*cleanup_func)(void *), void *free_arg);
void					free_commands_wrapper(void *arg);
// signal_handle.c
void					prmpt_ctrlc(int signal);
void					sig_init(void);
// ex
// \ execution.c
void					ft_execution(t_mini *mini, char **ev);
void					handle_command(t_mini *mini, t_command *cmd, char **ev,
							int has_next);
void					execute_direct(t_mini *mini, t_command *cmd, char **ev);
void					execute_in_child(t_mini *mini, t_command *cmd,
							char **ev, int has_next);
void					setup_redirections(t_mini *mini, t_command *cmd);
// \ execute.c
void					execution(t_mini *mini, t_command *cmd, char **ev);
char					*get_path(char *cmd, char **ev);
char					*cmd_path(char **ev);
// \ heredoc.c
char					*heredoc(t_mini *mini);
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
char					*get_env_var(t_list *env_list, char *var);
// \ export
int						ft_strlen_eq(char *s);
void					free_content(void *content);
void					delete_var(t_list **head, void *node_to_del);
void					bi_export(t_mini *mini, char **av);
// \ pwd
void					bi_pwd(void);
// \ unset
void					bi_unset(t_mini *mini, char **av);
// errors.c
void					error_msg(char *cmd, char *msg);
// colors.c
void					color_out(char *color, char *str);
#endif
