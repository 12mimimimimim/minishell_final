/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: didris <didris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 20:56:02 by didris            #+#    #+#             */
/*   Updated: 2025/09/28 20:56:02 by didris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "parser.h"
# include "signals.h"
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <termios.h>
# include "../utils/gnl/get_next_line.h"

/*
** STRUCT FOR ENV LIST
*/
typedef struct s_env
{
	char	*key;
	char	*value;
	int		exported;
}	t_env;

/*
** STRUCT TO CLEAN THE CHILD PROCESS
*/
typedef struct s_clean
{
	char	**split_argv;
	t_token	*tokens;
	t_ast	*main_node;
	t_env	**env_cpy;
}	t_clean;

/*
** BUILTIN FUNCTIONS
*/
int		builtin_cd(char **argv, t_env **env);
int		builtin_pwd(t_env **env);
int		builtin_exit(char **argv, t_clean *clean);
int		builtin_echo(char **argv);
int		builtin_export(t_env ***env, char **argv);
int		builtin_unset(t_env ***env, char **argv);
int		builtin_env(t_env **env, char **argv);
void	print_sorted_env(t_env **sorted);
int		find_eq(char *str);
int		is_numeric(char *str);
int		is_n_flag(char *str);
int		is_valid_key(char *str);
int		is_duplicate_unset(char **argv, int curr);
int		export_with_eq(t_env ***env, char *argv);
int		export_without_eq(t_env ***env, char *argv);
int		export_loop(t_env ***env, char *argv);
void	print_for_export(t_env **env);

/*
** REDIRECTION
*/
int		apply_redir(t_ast *node, t_env **env, t_clean *clean);
int		mult_heredoc(t_ast *node, t_env **env, int *last_fd, t_clean *clean);
t_ast	*find_last_input_redir(t_ast *node);
t_ast	*find_last_output_redir(t_ast *node);
int		validate_redirs(t_ast *node);
void	close_tree_heredocs(t_ast *node);
void	close_fd(int *last_fd);

/*
** COMMANDS
*/
void	child_process(t_ast *node, char **env_c, t_env **env, t_clean *clean);
int		parent_wait(pid_t pid);

/*
** PIPES
*/
void	pipe_l_child(t_ast *left, int *pipe_fds, t_env **env, t_clean *clean);
void	pipe_r_child(t_ast *right, int *pipe_fds, t_env **env, t_clean *clean);
int		pipe_parent(int l_pid, int r_pid, int *pipe_fds);
void	pipe_cleanup(t_ast *right, t_ast *left, int *pipefds, char *error);
int		handle_pipe_hd(t_ast *left, t_ast *right, t_env **env, t_clean *clean);

/*
** GENERAL EXECUTION
*/
int		execute_builtin(char **argv, t_env ***env, t_clean *clean);
int		execute(t_ast *node, t_env ***env, t_clean *clean);
int		is_parent_command(t_ast *node);
int		last_exit_status(int set, int value);
void	clean_child_exit(t_clean *clean, int exit_code);
t_ast	*find_cmd_node(t_ast *node);
void	close_fds(void);

/*
** ENVIRONMENT RELATED FUNCTIONS
*/
t_env	*create_env_node(char *env_str);
t_env	**init_env(char **env);
int		count_envp(char *envp[]);
int		find_env_index(t_env **env, char *key);
int		update_env(t_env **env, char *key, char *value);
int		add_to_env(t_env ***env, char *env_str, int exported);
int		remove_env(t_env **env, char *key);
char	**env_to_char(t_env **env);
void	sort_env(t_env **env);
t_env	**copy_env(t_env ***env, int count);
int		count_env_list(t_env **env);
void	free_env(t_env **env);
int		format_env_node(char **envp, t_env *node, int j);
t_env	*copy_env_var(t_env *src);
char	*prompt(int exit_code, t_env **env_copy);
void	shell_loop(t_env ***env_copy, int *exit_code);
int		is_valid_exit_arg(char *str, long long *result);

#endif
