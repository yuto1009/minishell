NAME     = minishell
CC       = cc
# -fsanitize=address
# RLDIR    = $(shell brew --prefix readline)
# INCLUDES = -I include -I$(RLDIR)/include
CFLAGS   = -Wall -Wextra -Werror -g -fsanitize=address -I include -I $(shell brew --prefix readline)/include
LDFLAGS  = -lreadline -L$(shell brew --prefix readline)/lib
SRCS     = src/main.c src/error.c src/tokenize.c built_in/builtin_exit.c built_in/builtin_env.c \
src/parser.c src/pipe_utils.c src/signal.c src/redirect.c \
src/tokenize_error.c src/serch_path.c src/exec_cmd.c src/wait_pid.c src/is_builtin.c src/exec_builtin.c built_in/echo.c built_in/pwd.c built_in/cd.c\


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

