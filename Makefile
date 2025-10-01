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

CORE_DIR		= $(SRC_DIR)core/
CORE_FILES		= $(CORE_DIR)shell_state.c

ENVIROMENT_DIR	= $(SRC_DIR)enviroment/
ENVIROMENT_FILES= $(ENVIROMENT_DIR)env_utils.c

ERROR_DIR		= $(SRC_DIR)error/
ERROR_FILES		= $(ERROR_DIR)error_manager.c\
				  $(ERROR_DIR)error_utils.c\
				  $(ERROR_DIR)error_builtin.c

SIGNAL_DIR		= $(SRC_DIR)signals/
SIGNAL_FILES	= $(SIGNAL_DIR)signal_handler.c\
				  $(SIGNAL_DIR)signal_utils.c

HISTORY_DIR		= $(SRC_DIR)history/
HISTORY_FILES	= $(HISTORY_DIR)history.c\
				  $(HISTORY_DIR)history_commands.c\
				  $(HISTORY_DIR)history_trim.c

LEXER_DIR		= $(SRC_DIR)lexer/
LEXER_FILES		= $(LEXER_DIR)lexer.c\
				  $(LEXER_DIR)lexer_word.c\
				  $(LEXER_DIR)lexer_operator.c\
				  $(LEXER_DIR)lexer_segments.c\
				  $(LEXER_DIR)lexer_aux.c\
				  $(LEXER_DIR)lexer_utils.c

PARSER_DIR		= $(SRC_DIR)command/
PARSER_FILES	= $(PARSER_DIR)cmd_parse.c \
				  $(PARSER_DIR)cmd_parse_token.c \
				  $(PARSER_DIR)cmd_parse_subshell.c \
				  $(PARSER_DIR)cmd_reviewer.c\
				  $(PARSER_DIR)cmd_reviewer_utils.c

REVIEWER_DIR	= $(SRC_DIR)reviewer/
REVIEWER_FILES	= $(REVIEWER_DIR)arg_builder.c\
				  $(REVIEWER_DIR)arg_token.c\
				  $(REVIEWER_DIR)expand_segments.c\
				  $(REVIEWER_DIR)expand_string.c\
				  $(REVIEWER_DIR)expand_var_utils.c\
				  $(REVIEWER_DIR)file_utils.c\
				  $(REVIEWER_DIR)mode_checkers.c\
				  $(REVIEWER_DIR)redir_file.c\
				  $(REVIEWER_DIR)token_redir.c

EXECUTOR_DIR	= $(SRC_DIR)executor/
EXECUTOR_FILES	= $(EXECUTOR_DIR)exe_builtin.c\
				  $(EXECUTOR_DIR)exe_external.c\
				  $(EXECUTOR_DIR)exe_logic.c\
				  $(EXECUTOR_DIR)exe_main.c\
				  $(EXECUTOR_DIR)exe_path.c\
				  $(EXECUTOR_DIR)exe_pipeline.c\
				  $(EXECUTOR_DIR)exe_pipe_child.c\
				  $(EXECUTOR_DIR)exe_pipe_main.c\
				  $(EXECUTOR_DIR)exe_pipe_parent.c\
				  $(EXECUTOR_DIR)exe_pipe_setup.c\
				  $(EXECUTOR_DIR)exe_pipe_setup2.c\
				  $(EXECUTOR_DIR)exe_redirect.c\
				  $(EXECUTOR_DIR)exe_redirect_utils.c\
				  $(EXECUTOR_DIR)exe_utils.c

BUILTIN_DIR		= $(SRC_DIR)builtins/
BUILTIN_FILES	= $(BUILTIN_DIR)builtin_cd.c\
				  $(BUILTIN_DIR)builtin_echo.c\
				  $(BUILTIN_DIR)builtin_env.c\
				  $(BUILTIN_DIR)builtin_exit.c\
				  $(BUILTIN_DIR)builtin_export.c\
				  $(BUILTIN_DIR)builtin_pwd.c\
				  $(BUILTIN_DIR)builtin_unset.c\
				  $(BUILTIN_DIR)builtin_utils.c

SRCS	=	$(CORE_FILES)\
			$(ENVIROMENT_FILES)\
			$(ERROR_FILES)\
			$(SIGNAL_FILES)\
			$(HISTORY_FILES)\
			$(LEXER_FILES)\
			$(PARSER_FILES)\
			$(REVIEWER_FILES)\
			$(EXECUTOR_FILES)\
			$(BUILTIN_FILES)\
			$(SRC_DIR)main.c\
			$(SRC_DIR)redirections.c

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

