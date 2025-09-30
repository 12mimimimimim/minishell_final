/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: didris <didris@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 21:39:32 by didris            #+#    #+#             */
/*   Updated: 2025/09/28 21:39:32 by didris           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "execute.h"

typedef struct s_exp_var
{
	char	*result;// current write position
	int		flag;// expansion flag
}	t_exp_var;

//EXPANDER_UTILS.C
char	*ft_strim(const char *s);
char	*get_env_value(const char *key, t_env **env);
int		is_valid_var_char(char c);
char	*extract_key(const char *str);
char	*expand_value(char *str, t_env **env);
int		is_quoted(const char *s);
int		count_args(char **argv);

//EXPANDER/EXPANDER.C
char	**expand_variable(t_ast *find_var, t_env **env, char **split_argv);
char	*process_string_expansion(char *str, t_env **env);
char	*process_heredoc_line(char *line, int expand, t_env **env);
int		process_argv_array(char **argv, t_env **env);
int		process_filename(t_ast *node, t_env **env);
int		is_redirection_node(t_ast_node_type type);
int		process_child_nodes(t_ast *node, t_env **env);
char	*expand_value(char *string, t_env **env);
int		expand_command_node(t_ast *which_node, t_env **env);
int		expand_redirection_node(t_ast *which_node, t_env **env);
t_ast	*expand_node(t_ast *which_node, t_env **env);
t_ast	*expander(t_ast *which_node, t_env **env);

//expander/count_args_length.c
int		process_single_quote(char *string, int *i, int *total_len);
int		process_double_quote(char *string, int *i, t_env **env, int hd_delim);
int		process_variable(char *string, int *i, int *total_len, t_env **env);
int		get_expanded_length(char *string, int *i, t_env **env);
int		count_trimmed_quotes(char *string, char c);
int		count_new_args_length(char *string, t_env **env, int hd_delim);

//expander/copy_args_length.c
int		copy_single_quoted(char *string, int *i, t_exp_var *more);
int		copy_double_quoted(char *string, int *i, t_exp_var *more, t_env **env);
int		copy_variable(char *string, int *i, t_exp_var *more, t_env **env);
int		process_character(char *string, int *i, t_exp_var *more, t_env **env);
char	*process_mixed_args(char *string, t_env **env, int flag);
void	skip_variable_name(char *string, int *i);
int		handle_dollar_expansion(char *string, int *i, char *result,
			t_env **env);
void	clean_env_exit(int exit_code, t_env **env_copy);
void	cleanup_iteration(char **line, char ***argv, t_token **tokens,
			t_ast **ast);

#endif
