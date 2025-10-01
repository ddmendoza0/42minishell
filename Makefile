# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/08 18:40:05 by dmendoza          #+#    #+#              #
#    Updated: 2025/09/30 15:07:11 by dmaya-vi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	minishell

HEADERS	=	-I ./include -I -I$(LIBFT)

LIBFT	=	./lib/libft
LIBS = $(LIBFT)/libft.a -L/mingw64/lib -lreadline


SRC_DIR =	src/

ERROR_DIR	= $(SRC_DIR)error/
ERROR_FILES	= $(ERROR_DIR)error_manager.c\
			  $(ERROR_DIR)error_utils.c\
			  $(ERROR_DIR)error_builtin.c

SRCS	=	$(ERROR_FILES)\
			$(SRC_DIR)main.c\
			$(SRC_DIR)shell_state.c\
			$(SRC_DIR)lexer.c\
			$(SRC_DIR)lexer_word.c\
			$(SRC_DIR)lexer_operator.c\
			$(SRC_DIR)lexer_segments.c\
			$(SRC_DIR)lexer_aux.c\
			$(SRC_DIR)lexer_utils.c\
			$(SRC_DIR)history.c\
			$(SRC_DIR)history_commands.c\
			$(SRC_DIR)cmd_parse.c \
			$(SRC_DIR)cmd_parse_token.c \
			$(SRC_DIR)cmd_parse_subshell.c \
			$(SRC_DIR)cmd_reviewer.c\
			$(SRC_DIR)cmd_reviewer_utils.c\
			$(SRC_DIR)token_management.c\
			$(SRC_DIR)env_utils.c\
			$(SRC_DIR)builtin_cd.c\
			$(SRC_DIR)builtin_pwd.c\
			$(SRC_DIR)builtin_echo.c\
			$(SRC_DIR)builtin_export.c\
			$(SRC_DIR)builtin_unset.c\
			$(SRC_DIR)builtin_env.c\
			$(SRC_DIR)builtin_exit.c\
			$(SRC_DIR)builtin_utils.c\
			$(SRC_DIR)cmd_executioner.c\
			$(SRC_DIR)redirections.c\
			$(SRC_DIR)signal_handler.c\
			$(SRC_DIR)history_trim.c\

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

