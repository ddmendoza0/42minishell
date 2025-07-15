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
const char *token_type_to_str(t_token_type type)
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
	if (type == SEMICOLON)
		return "SEMICOLON";
        return "UNKNOWN";
}

const char *quote_type_to_str(t_quote_type type)
{
        if (type == QUOTE_NONE)
                return "NONE";
        if (type == QUOTE_SINGLE)
                return "SINGLE";
        if (type == QUOTE_DOUBLE)
                return "DOUBLE";
        return "UNKNOWN";
}

void print_tokens(t_token *list)
{
        int i = 0;

        while (list)
        {
                printf("Token %d:\n", i);
                printf("  Type : %s\n", token_type_to_str(list->type));
                printf("  Value: [%s]\n", list->value);
                if (list->type == WORD)
                        printf("  Quote: %s\n", quote_type_to_str(list->quote_type));
                printf("\n");
                list = list->next;
                i++;
        }
}
//end testing

static int is_operator(const char *input)
{
	if (!input)
		return (0);
	if (strncmp(input, "&&", 2) == 0)
		return (1);
	if (strncmp(input, "||", 2) == 0)
		return (1);
	if (strncmp(input, ">>", 2) == 0)
		return (1);
	if (strncmp(input, "<<", 2) == 0)
		return (1);
	if (*input == '|' || *input == '<' || *input == '>' || *input == ';' ||	\
		*input == '(' || *input == ')' || *input == ';')
		return (1);
	return (0);
}

static int is_word_boundary(char c)
{
	return (ft_isspace(c) || is_operator(&c) || c == '\0');
}

static void	skip_quotes(size_t *i, const char *input)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
}

static void	ext_word_token(size_t *i, const char *input, t_token **lst)
{
	size_t	start;
	char	*word;

	start = *i;
	while (input[*i] && !is_word_boundary(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
			skip_quotes(i,input);
		else
			(*i)++;
	}
	word = ft_strndup(input + start, *i - start);
	if (!word)
		return ;
	addback_token(lst, create_token(word, WORD));
}

//gestionar el non closing quote
static void	ext_quoted_token(size_t *i, const char *input, t_token **lst)
{
	size_t	start;
	char	*word;
	char	quote;
	t_quote_type	q_type;
	t_token	*token;

	quote = input[*i];
	if (quote == '\'')
		q_type = QUOTE_SINGLE;
	else
		q_type = QUOTE_DOUBLE;
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
	{
		word = ft_strndup(input + start, *i - start);
		(*i)++;
	}
	else
		word = ft_strndup(input + start, *i - start);
	if (!word)
		return ;
	token = create_token(word, WORD);
	if (token)
		token->quote_type = q_type;
	addback_token(lst, token);
}

static t_token_type	det_op_type(const char *op)
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
	else if (ft_strncmp(op, ";", 1) == 0)
		return (SEMICOLON);
	else
		return (WORD);
}

static void	ext_operator_token(size_t *i, const char *input, t_token **lst)
{
	char	*op;

	op = NULL;
	if (input[*i] == '>' && input[*i + 1] == '>')
	{
		op = ft_strndup(">>", 2);
		*i += 2;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		op = ft_strndup("<<", 2);
		*i += 2;
	}
	else if (input[*i] == '|' && input[*i + 1] == '|')
	{
		op = ft_strndup("||", 2);
		*i += 2;
	}
	else if (input[*i] == '&' && input[*i + 1] == '&')
	{
		op = ft_strndup("&&", 2);
		*i += 2;
	}
	else
	{
		op = ft_strndup(&input[*i], 1);
		(*i)++;
	}
	if (!op)
		return ;
	addback_token(lst, create_token(op, det_op_type(op)));
}

t_token	*lexer(char *input)
{
	size_t	i;
	t_token	*token_lst;

	token_lst = NULL;
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (input[i] == '\'' || input[i] == '"')
			ext_quoted_token(&i, input, &token_lst);
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>' \
				|| input[i] == '&' || input[i] == ';')
			ext_operator_token(&i, input, &token_lst);
		else
			ext_word_token(&i, input, &token_lst);
	}
	print_tokens(token_lst);
	return (token_lst);
}
