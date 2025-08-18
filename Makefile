# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/08 18:40:05 by dmendoza          #+#    #+#              #
#    Updated: 2025/07/15 17:31:41 by dmendoza         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

HEADERS	=	-I ./include -I -I$(LIBFT)

LIBFT	=	./lib/libft
LIBS = $(LIBFT)/libft.a -L/mingw64/lib -lreadline

SRC_DIR =	src/
SRCS	=	$(SRC_DIR)main.c\
			$(SRC_DIR)shell_state.c\
			$(SRC_DIR)error_manager.c\
			$(SRC_DIR)lexer.c\
			$(SRC_DIR)lexer_aux.c\
			$(SRC_DIR)history.c\
			$(SRC_DIR)history_commands.c\
			$(SRC_DIR)cmd_parse.c \
			$(SRC_DIR)cmd_parse_argv.c \
			$(SRC_DIR)cmd_parse_redir.c \
			$(SRC_DIR)cmd_reviewer.c \
			$(SRC_DIR)env_utils.c\

OBJ_DIR =	obj/
OBJS	=	$(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

INCLUDE =	include/minishell.h Makefile

CC	=	cc
CFLAGS	=	-g -Wall -Wextra -Werror -Wunreachable-code -Ofast -I./libft \
		-I./include -I$(LIBFT) -I/mingw64/include
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

