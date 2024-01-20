NAME     = minishell
CC       = cc
CFLAGS   = -Wall -Wextra -Werror
LDFLAGS  = -lreadline 
SRCS     = src/main.c src/error.c src/tokenize.c built_in/builtin_exit.c built_in/builtin_env.c src/parser.c\

OBJS = $(SRCS:.c=.o)

LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

all: $(LIBFT) $(NAME) 

$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(NAME) $(OBJS) $(LIBFT)
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