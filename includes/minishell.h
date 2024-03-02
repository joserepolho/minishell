/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoribe <joaoribe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 20:27:57 by tiagoliv          #+#    #+#             */
/*   Updated: 2024/03/02 05:51:17 by joaoribe         ###   ########.fr       */
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
# define SLASH '/'
# define SLASH_STR "/"
# define ENV_VAR '$'
# define TILDE '~'
/* Last command exit status */
# define ENV_Q '?'
# define SPACE ' '

# define ECHO_FLAG_N "-n"

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
# define TOO_MANY_ARGS "too many arguments\n"
# define NOT_VALID_IDENT "not a valid identifier: "
# define OPEN_QUOTES_ERROR "open quotes are not supported!\n"
# define CMD_INSUF_PERMS "Permission denied: "
# define EXIT_NUMERIR_ARG_REQ "exit: numeric argument required: "

# define CMD_NOT_FOUND_RET 127
# define CMD_INSUF_PERMS_RET 126
# define CMD_NUM_ARG_REQ_RET 2

// free shell errors
# define FAILURE_GETTING_PATH "Failure getting path!\n"
# define DIR_CHANGE_ERROR "Directory change failure!\n"
# define PIPE_ERROR "Pipe error!\n"
# define FORK_ERROR "Fork error!\n"

# define EXIT_STATUS_N 256

# define DEBUG_MSG(fmt, ...) printf("[%s::%s::%d]:" fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

enum					e_redir_type
{
	RED_NULL = 0,
	RED_IN,
	RED_AIN,
	RED_OUT,
	RED_AOUT
};

typedef void			(*t_cleanup_func)(void *);

typedef struct s_str_ex
{
	char				*res;
	int					len;
	int					i;
	bool				quotes;
	bool				dquotes;
	int					var_len;
	int					var_clen;
	char				*var;
}						t_str_ex;

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
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				*cmd_name;
	char				**args;
	t_redir				*redirs;
	int					status;
	struct s_command	*next;
}						t_command;

typedef struct s_mini
{
	t_input				input;
	t_command			*commands;
	int					command_ret;
	t_list				*env_list;
	char				*hd_limiter;
	int					lim_q;
	char				*output;
	int					hdfd;
}						t_mini;

// main.c
int						main(int ac, char **av, char **env);

// input.c
char					*get_input(bool prompt);
void					update_prompt(void);

// errors.c
void					error_msg(char *cmd, char *msg);
void					error_msg_ret(char *cmd, char *msg, int ret);

// path.c
char					*get_roa_path(char *cmd);
char					*get_envpath_cmd(char *cmd);
char					*path_add_home_prefix(char *cmd);
char					*get_relative_path(char *cmd);
char					*get_cmd_path(char *cmd);
// path_utils.c
bool					is_relative_path(char *cmd);
bool					is_absolute_path(char *cmd);
int						can_access_path(char *path);
int						can_path_to(char *path);
char					*get_env_value(t_list *envs, char *key);
// mini.c
void					init_mini(t_mini *mini);
t_mini					*mini(void);
void					reset_mini(t_mini *mini);
t_list					*set_env(char **env);

// utils.c
enum e_redir_type		redir_type(char *line);
bool					valid_env_char(char c);
bool					quoted_str(char *str);
bool					valid_cmd_arg(char *str);
void					free_assign_null(void **ptr);
void					print_command(t_command *command);
// pl
//  \ lexer.c
bool					input_error_check(t_mini *mini);
bool					skip_spaces(char **line);
bool					semantic_checker(char **raw_commands);
bool					valid_section(char **sections, int *i,
							char **last_section, char **error);
//  \ parser.c
bool					parse_input(t_mini *mini);
size_t					parse_size(char *line);
char					*get_next_section(char **line);
char					**parse(t_mini *mini);
t_command				*construct_command(char **raw_commands, size_t end);
//  \ parser_helpers.c
size_t					redir_size(char *line);
void					command_add_back(t_command *new_command);
bool					assign_redir(t_command *command, char *redir_file,
							enum e_redir_type type);
bool					update_command(t_command *command, char **raw_commands,
							size_t *i, size_t end);
bool					add_arg(t_command *command, char *section);
// \ str_expander.c
char					*str_expander(char *str);
void					expand_vars(char *str, char *expanded, int len);
int						str_expander_len(char *str);
char					*str_expander_var_len(t_str_ex *ex, char *str);
// \ str_expander_utils.c
bool					expand_command(t_command *cmd, char **ev);
void					expand_args(t_command *cmd);
bool					expand_redirs(t_command *cmd);
// free.c
void					free_commands(t_command *commands);
void					free_redirs(t_redir *redirs);
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
void					execute_in_child(t_command *cmd, char **ev,
							int has_next);
void					execute_in_parent(t_mini *mini, t_command *cmd,
							int has_next);
void					setup_redirections(t_command *cmd, bool isparent);
void					wait_for_children(t_mini *mini, int i);
// \ execute.c
bool					execution(t_command *cmd, char **ev);
// \ heredoc.c
char					*heredoc(t_mini *mini);
// b-ins
// \ utils.c
int						if_builtin(char *s);
void					built_in(t_mini *mini, t_command *cmd);
// \ cd
void					bi_cd(t_mini *mini, char **av);
// \ echo
void					bi_echo(char **av);
// \ env
void					bi_env(t_list *env_list);
char					*get_env_var(t_list *env_list, char *var);
int						valid_env_var_name(char *str, bool is_entry);
// \ export
int						ft_strlen_eq(char *s);
void					delete_var(t_list **head, void *node_to_del);
void					show_export(t_list *env_list, char **av);
void					bi_export(t_mini *mini, char **av);
// \ pwd
void					bi_pwd(void);
// \ unset
void					bi_unset(t_mini *mini, char **av);
// \ exit
int						calculate_exit_code_from_string(const char *number);
bool					str_is_num(const char *str);
bool					bi_exit(t_mini *mini, char **args, bool has_next);
#endif
