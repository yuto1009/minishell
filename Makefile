NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline
SRCS = src/main.c src/tokenize.c src/error.c src/destructor.c src/expand.c src/parse.c
OBJS = $(SRCS:.c=.o)

LIBFT_PATH = ./libft
LIBFT = $(LIBFT_PATH)/libft.a

all: $(LIBFT) $(NAME) 

$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
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
