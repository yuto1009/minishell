NAME     = minishell
CC       = cc
# -fsanitize=address
# RLDIR    = $(shell brew --prefix readline)
# INCLUDES = -I include -I$(RLDIR)/include
CFLAGS   = -Wall -Wextra -Werror -g -fsanitize=address -I include -I $(shell brew --prefix readline)/include
LDFLAGS  = -lreadline -L$(shell brew --prefix readline)/lib
SRCS     = src/main.c src/error.c src/tokenize.c built_in/builtin_exit.c built_in/builtin_env.c src/parser.c src/pipe_utils.c src/signal.c\

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

