NAME	= minishell
CC		= cc
CFLAGS	= -Wall -Werror -Wextra -Iincludes -I$(LIBFT_DIR)
RM		= rm -f

LIBFT_DIR = utils/libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS	= check_if_valid_cmd/cmd_path.c 	\
		  cleaner/cleaner.c					\
		  cleaner/cleaner_utils.c			\
		  env/edit_env.c					\
		  env/env_utils.c					\
		  env/env.c 						\
		  executer/execute.c 				\
		  executer/builtin/builtin_export.c	\
		  executer/builtin/builtin_utils.c	\
		  executer/builtin/builtin.c		\
		  executer/builtin/parent_builtin.c	\
		  executer/builtin/exit_utils.c		\
		  executer/redirection/execute_redir.c\
		  executer/redirection/heredoc.c	\
		  executer/redirection/redir_utils.c\
		  executer/cmd_child.c				\
		  executer/execute_pipe.c			\
		  executer/execute_utils.c			\
		  expander/expander.c 				\
		  expander/count_args_length.c 		\
		  expander/count_args_length_utils.c\
		  expander/copy_args_length_utils.c	\
		  expander/copy_args_length.c		\
		  expander/expand.c 				\
		  expander/expander_utils.c 		\
		  lexer/init_token.c 				\
		  parser/parser.c 					\
		  parser/parse_command.c			\
		  parser/parse_redirection.c		\
		  parser/parse_command_utils.c		\
		  get_input/check_syntax_error.c	\
		  get_input/check_syntax_error_utils.c \
		  get_input/check_pipes.c			\
		  get_input/get_input_utils.c		\
		  get_input/split_input.c			\
		  get_input/get_input.c				\
		  get_input/handle_quoted_word.c 	\
		  utils/utils.c 					\
		  signals.c 						\
		  utils/gnl/get_next_line.c			\
		  main/loop.c						\
		  main/main.c						\
		  main/prompt.c						\

OBJS	= $(SRCS:.c=.o)

LIBS	= -lreadline -L/opt/vagrant/embedded/lib/ -I/opt/vagrant/embedded/include/readline

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LIBS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS)
	$(RM) $(NAME) 
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
