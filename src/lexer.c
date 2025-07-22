/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:46:37 by dmendoza          #+#    #+#             */
/*   Updated: 2025/07/15 18:31:32 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//testing
const char* token_type_to_str(t_token_type type)
{
	if (type == WORD)
		return "WORD";
	if (type == PIPE)
		return "PIPE";
	if (type == REDIRECT_IN)
		return "REDIRECT_IN";
	if (type == REDIRECT_OUT)
		return "REDIRECT_OUT";
	if (type == APPEND_OUT)
		return "APPEND_OUT";
	if (type == HEREDOC)
		return "HEREDOC";
	if (type == AND)
		return "AND";
	if (type == OR)
		return "OR";
	if (type == LPAREN)
		return "LPAREN";
	if (type == RPAREN)
		return "RPAREN";
	if (type == EXPAND)
		return "EXPAND";
	return "UNKNOWN";
}

void print_tokens(t_token** list)
{
	int i = 0;

	t_token* curr = *list;
	while (curr)
	{
		printf("Token %d:\n", i);
		printf("  Type : %s\n", token_type_to_str(curr->type));
		printf("  Value: [%s]\n", curr->value);
		printf("\n");
		curr = curr->next;
		i++;
	}
}
//end testing

static int is_single_char_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' || c == '(' || c == ')');
}

static int is_multi_char_operator(const char* input)
{
	return (ft_strncmp(input, "&&", 2) == 0 || ft_strncmp(input, "||", 2) == 0 \
		|| ft_strncmp(input, ">>", 2) == 0 || ft_strncmp(input, "<<", 2) == 0);
}

static t_token_type	det_op_type(const char* op)
{
	if (ft_strncmp(op, "||", 2) == 0)
		return (OR);
	else if (ft_strncmp(op, "|", 1) == 0)
		return (PIPE);
	else if (ft_strncmp(op, ">>", 2) == 0)
		return (APPEND_OUT);
	else if (ft_strncmp(op, ">", 1) == 0)
		return (REDIRECT_OUT);
	else if (ft_strncmp(op, "<<", 2) == 0)
		return (HEREDOC);
	else if (ft_strncmp(op, "<", 1) == 0)
		return (REDIRECT_IN);
	else if (ft_strncmp(op, "&&", 2) == 0)
		return (AND);
	else if (ft_strncmp(op, "(", 1) == 0)
		return (LPAREN);
	else if (ft_strncmp(op, ")", 1) == 0)
		return (RPAREN);
	else
		return (WORD);
}

static int extract_word_token(size_t* i, const char* input, t_token** lst, t_shell_state* shell)
{
	size_t start = *i;
	char* word;
	t_token* token;

	while (input[*i] && !ft_isspace(input[*i]) && !is_single_char_operator(input[*i]))
		(*i)++;
	word = ft_strndup(input + start, *i - start);
	if (!word)
	{
		set_error(shell, ERR_MALLOC, "word token");
		free_token_lst(*lst);
		*lst = NULL;
		return (0);
	}
	token = create_token(word, WORD);
	if (!token)
	{
		set_error(shell, ERR_MALLOC, "word token");
		free(word);
		free_token_lst(*lst);
		*lst = NULL;
		return (0);
	}
	addback_token(lst, token);
	return (1);
}

static int extract_operator_token(size_t* i, const char* input, t_token** lst, t_shell_state* shell)
{
	char* op;
	t_token* token;

	if (is_multi_char_operator(&input[*i]))
		op = ft_strndup(&input[*i], 2), * i += 2;
	else
		op = ft_strndup(&input[*i], 1), (*i)++;
	if (!op)
	{
		set_error(shell, ERR_MALLOC, "operator token");
		free_token_lst(*lst);
		*lst = NULL;
		return (0);
	}
	token = create_token(op, det_op_type(op));
	if (!token)
	{
		set_error(shell, ERR_MALLOC, "operator token");
		free(op);
		free_token_lst(*lst);
		*lst = NULL;
		return (0);
	}
	addback_token(lst, token);
	return (1);
}

int lexer(char* input, t_token** token_lst, t_shell_state* shell)
{
	size_t	i;
	int		success;

	if (!input || !token_lst || !shell)
		return (0);
	*token_lst = NULL;
	clear_error(shell);
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (!input[i])
			break;
		if (is_single_char_operator(input[i]))
			success = extract_operator_token(&i, input, token_lst, shell);
		else
			success = extract_word_token(&i, input, token_lst, shell);
		if (!success)
			return (0);
	}
	print_tokens(token_lst); // Remove this in production
	return (1);
}
