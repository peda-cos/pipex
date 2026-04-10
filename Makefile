# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+             #
#    Created: 2026/04/10 00:00:00 by peda-cos          #+#    #+#              #
#    Updated: 2026/04/10 00:00:00 by peda-cos         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = pipex
CC          = cc
CFLAGS      = -Wall -Wextra -Werror
INC         = -I include -I libft
LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a

SRCS        = src/pipex.c \
              src/children.c \
              src/path.c \
              src/error.c

BONUS_SRCS  = bonus/pipex_bonus.c \
              bonus/children_bonus.c \
              bonus/heredoc_bonus.c \
              bonus/path_bonus.c \
              bonus/error_bonus.c

OBJS        = $(SRCS:.c=.o)
BONUS_OBJS  = $(BONUS_SRCS:.c=.o)

all: $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)
	@echo "Compiled: $(NAME)"

%.o: %.c
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@
	@echo "Compiling: $<"

bonus: .bonus

.bonus: $(LIBFT) $(BONUS_OBJS)
	@$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -o $(NAME)
	@echo "Compiled: $(NAME) (bonus)"
	@touch .bonus

clean:
	@rm -f $(OBJS) $(BONUS_OBJS)
	@make -C $(LIBFT_DIR) clean
	@echo "Cleaned: objects"

fclean: clean
	@rm -f $(NAME) .bonus
	@make -C $(LIBFT_DIR) fclean
	@echo "Cleaned: $(NAME)"

re: fclean all

.PHONY: all clean fclean re bonus
