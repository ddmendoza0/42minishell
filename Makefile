# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/08 18:40:05 by dmendoza          #+#    #+#              #
#    Updated: 2025/07/10 13:36:22 by dmaya-vi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

HEADERS	=	-I ./include -I -I$(LIBFT)

LIBFT	=	./lib/libft
LIBS	=	$(LIBFT)/libft.a -lreadline

SRC_DIR =	src/
SRCS	=	$(SRC_DIR)main.c\
		$(SRC_DIR)history.c\

OBJ_DIR =	obj/
OBJS	=	$(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

INCLUDE =	include/minishell.h Makefile

CC	=	cc
CFLAGS	=	-g -Wall -Wextra -Werror -Wunreachable-code -Ofast -I./libft \
		-I./include -I$(LIBFT)
RM	=	rm -f

all:	libft $(NAME)

libft:		$(LIBFT)/libft.a

$(LIBFT)/libft.a: FORCE 
	@$(MAKE) -C $(LIBFT)

$(NAME):	$(OBJS)
		@echo "Building minishell..."
		@$(CC) $(OBJS) $(LIBS) -o $(NAME) 
		@echo "...minishell built successfully."

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c $(INCLUDE)
		@mkdir -p $(dir $@)
		@echo "Compiling $<..."
		@$(CC) $(CFLAGS) -c $< -o $@
		@echo "...compilation completed"
clean:
	@echo "Cleaning object files..."
	@$(RM) $(OBJS)
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT) clean
	@echo "...object files cleansed."

fclean:	clean
	@echo "Cleaning all..."
	@$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT) fclean
	@echo "...all cleaned."

re:	fclean all

.PHONY:	all clean fclean re libft libmlx FORCE

