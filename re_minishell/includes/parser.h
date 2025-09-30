/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vmorena <vmorena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 20:14:49 by vmorena           #+#    #+#             */
/*   Updated: 2025/09/29 21:10:33 by vmorena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <string.h>
# include "libft.h"
# include "signals.h"

# define QUOTELESS -2
# define SUCCESS 1
# define FAILURE 0

/* ============================ TOKEN TYPES ================================ */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

/* ============================= AST TYPES ================================= */
typedef enum e_ast_node_type
{
	AST_COMMAND,
	AST_PIPE,
	AST_REDIR_IN,
	AST_REDIR_OUT,
	AST_REDIR_APPEND,
	AST_HEREDOC
}	t_ast_node_type;

/* ============================= TOKEN ===================================== */
typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}	t_token;

/* ============================== AST ====================================== */
typedef struct s_ast
{
	t_ast_node_type		type;
	char				**argv;
	struct s_ast		*left;
	struct s_ast		*right;
	char				*filename;
}	t_ast;

/* ============================= WORD COUNT ================================ */
typedef struct s_wc_state
{
	int	total_word_count;
	int	in_word;
}	t_wc_state;

/* ===================== get_input/check_syntax_error.c ==================== */
int				skip_whitespace(char *string, int start);
int				is_single_redirection(char c);
int				get_redirection_length(char *string, int i);
int				is_pipe(char c);
void			syntax_error(char *which_token);
int				check_pipe_syntax(char *read_line, int *i);
int				check_redirection_syntax(char *read_line, int *i);
int				check_for_bash_syntax_error(char *read_line);

/* ==================== get_input/handle_quoted_word.c ===================== */
int				is_single_quote(char c);
int				is_double_quote(char c);
int				handle_quoted_word(char *read_line, int current_i, char c);

/* ======================== get_input/get_input.c ========================== */
int				count_rl_contents(char *read_line, char delimiter);
int				process_quoted_word(char *read_line, int *i,
					int *total_word_count);
int				process_regular_word(char *read_line, int i, char del,
					int *in_word);
int				find_word_end(char *read_line, int start, char delimeter);
char			*get_characters(char *read_line, int *current_i,
					char delimeter);
char			**allocate_result_array(char *read_line, char delimeter);
int				get_words(char *read_line, char delimeter, char **result);
char			**split_argv_contents(char *read_line, char delimeter);

/* ========================= get_input/print_error ========================= */
int				print_error(char *code);

/* ========================= lexer/init_token.c ============================ */
t_token_type	get_token_type(char *value);
void			add_token_to_list(t_token **first, t_token **last,
					t_token *new_token);
t_token			*create_token(char *value);
t_token			*tokenator(char **argv);

/* ======================== parser/parse_command.c ========================= */
int				count_commands(t_token **tokens);
int				copy_word_to_argv(char **argv, int *i, t_token **current);
void			skip_redirection_filename(t_token **current);
int				fill_argv_array(char **argv, t_token *tokens, int count);
char			**collect_commands(t_token **tokens);
t_ast			*parse_commands(t_token **tokens);

/* ===================== parser/parse_redirection.c ======================== */
t_ast			*grab_filename(t_token **tokens);
t_ast			*handle_command_or_redirection(t_token **tokens);

/* ========================= parser/parser.c =============================== */
t_ast			*ast_initializer(t_ast_node_type which_type);
t_ast			*parse_pipeline(t_ast *left_node, t_token **tokens);
t_ast			*parser(t_token **tokens);

/* ======================== parser/print_ast.c ============================= */
void			print_expanded_argv(t_ast *ast);
void			print_node(t_ast *node);
void			pretty_print_ast(t_ast *node, const char *prefix, int is_left);
void			print_args(char **args);

/* ========================= cleaner/cleaner.c ============================= */
void			free_token(t_token *token);
void			free_tokens(t_token *tokens);
void			free_argv(char **argv);
void			free_ast(t_ast *node);
void			cleanup_partial_argv(char **argv, int count);
void			cleanup_parser_error(t_ast *ast, t_token *tokens);

/* ============================= ENV ======================================= */
int				count_envp(char *envp[]);
char			**copy_envp(char *envp[]);
int				find_envp_index(char *envp[], char *key);
char			*create_envp_var(char *key, char *value);
int				update_envp(char *envp[], char *key, char *value);
int				add_to_envp(char **envp[], char *key, char *value);
int				remove_envp(char **envp[], char *key);
void			sort_envp(char *envp[]);

/* ===================== check_if_valid_cmd/cmd_path.c ===================== */
char			*ft_strcpy(char *dest, const char *src);
int				is_executable_path(char *path);
char			*find_cmd_in_path(char *cmd);
char			*dir_and_cmd_combo_path(char *dir, char *cmd);
char			*validate_cmd(char *cmd);

/* ================= check_if_valid_cmd/filter_cmd_path.c ================== */
char			*build_path(const char *dir, const char *cmd);
char			*get_next_path_dir(char *path_env, int *start);
int				command_exists_in_path(const char *command);

/* ======================= validate_redir_target.c ========================= */
int				is_valid_file(const char *file);
int				is_a_valid_redir_target(char *filename);
int				is_valid_directory(const char *path);
int				create_file(const char *filename);

/* ============================== utils.c ================================== */
void			free_ast(t_ast *node);
char			*ft_strndup(const char *s, size_t n);
int				last_exit_status(int set, int value);
int				is_builtin(const char *value);
void			increment_var(int *total_word_count, int *in_word, char *code);
int				process_character_wc(char *read_line, int *i, char delimiter,
					t_wc_state *st);
t_ast			*process_redirections(t_token **tokens, t_ast *result_ast);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
void			ft_putstr_fd(char *s, int fd);
int				executable_code(char *code, char *path);
void			cleanup_iteration(char **line, char ***argv, t_token **tokens,
					t_ast **ast);
void			close_fds(void);

#endif
