NAME     = minishell
CC       = cc -fsanitize=address
# RLDIR    = $(shell brew --prefix readline)
# INCLUDES = -I include -I$(RLDIR)/include
CFLAGS   = -Wall -Wextra -Werror -g -I include -I $(shell brew --prefix readline)/include
LDFLAGS  = -lreadline -L$(shell brew --prefix readline)/lib
SRCS     = src/main.c src/interpret.c src/error.c src/tokenize.c built_in/builtin_exit.c built_in/builtin_env.c \
src/parser.c src/pipe_utils.c src/signal.c src/redirect.c \
src/tokenize_error.c src/search_path.c src/exec_cmd.c src/wait_pid.c src/expand.c src/map.c built_in/builtin_export.c \
built_in/builtin_unset.c built_in/builtin_echo.c built_in/builtin_pwd.c built_in/builtin_cd.c built_in/builtin_cd_utils.c built_in/error_builtin.c src/map_get.c src/map_init.c src/trim_env.c\
src/tokenize_is.c src/tokenize_line.c src/tokenize_new.c src/tokenize_operator.c src/tokenize_word.c src/headoc.c src/exec_utils.c\
src/expand_append_utils.c src/expand_append.c src/expand_is.c src/expand_append_env.c src/expand_remove.c src/error_exit.c src/signal_init.c
OBJS = $(SRCS:.c=.o)

LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

all: $(LIBFT) $(NAME) 

$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LDFLAGS) 
	@echo "Successfully compiled $(NAME)"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_PATH) clean
	@echo $@

fclean:
	@rm -f $(OBJS)
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_PATH) fclean
	@echo $@

re: fclean all

.PHONY: all clean fclean re
