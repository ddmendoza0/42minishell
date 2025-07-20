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

const char* quote_type_to_str(t_quote_type type)
{
	if (type == QUOTE_NONE)
		return "NONE";
	if (type == QUOTE_SINGLE)
		return "SINGLE";
	if (type == QUOTE_DOUBLE)
		return "DOUBLE";
	return "UNKNOWN";
}

void print_tokens(t_token **list)
{
	int i = 0;

	t_token* curr = *list;
	while (curr)
	{
		printf("Token %d:\n", i);
		printf("  Type : %s\n", token_type_to_str(curr->type));
		printf("  Value: [%s]\n", curr->value);
		if (curr->type == WORD)
			printf("  Quote: %s\n", quote_type_to_str(curr->quote_type));
		printf("\n");
		curr = curr->next;
		i++;
	}
}
//end testing

static int is_operator(const char* input)
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
	if (*input == '|' || *input == '<' || *input == '>' || *input == '(' || *input == ')')
		return (1);
	return (0);
}

static int is_word_boundary(char c)
{
	return (ft_isspace(c) || is_operator(&c) || c == '\0');
}

static void	skip_quotes(size_t* i, const char* input)
{
	char	quote;

	quote = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
		(*i)++;
}

static int	extract_word_token(size_t* i, const char* input, t_token** lst)
{
	size_t	start;
	char* word;

	start = *i;
	while (input[*i] && !is_word_boundary(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
			skip_quotes(i, input);
		else
			(*i)++;
	}
	word = ft_strndup(input + start, *i - start);
	if (!word)
		return (1);
	addback_token(lst, create_token(word, WORD));
	return (1);
}

static int	extract_quoted_token(size_t* i, const char* input, t_token** lst)
{
	size_t	start;
	char* word;
	char	quote;
	t_quote_type	q_type;
	t_token* token;

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
		if (!word)
		{
			free_token_lst(*lst);
			*lst = NULL;
			return (0);
		}
		(*i)++;
	}
	else //handle a global error flag or sth like that
	{
		printf("minishell: syntax error: unclosed quote\n");
		return (0);
	}
	if (!word)
		return (1);
	token = create_token(word, WORD);
	if (token)
		token->quote_type = q_type;
	addback_token(lst, token);
	return (1);
}

static int extract_dollar_token(size_t* i, const char* input, t_token** lst)
{
	size_t start;
	char* var_name;

	start = *i;
	(*i)++; // Skip the '$'

	// Handle $? specially
	if (input[*i] == '?')
	{
		(*i)++;
		var_name = ft_strndup(input + start, *i - start);
		if (!var_name)
			return (1);
		addback_token(lst, create_token(var_name, EXPAND));
		return (1);
	}
	// Extract variable name (alphanumeric + underscore)
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;

	// If no valid variable name after $, treat $ as literal
	if (*i == start + 1)
	{
		var_name = ft_strndup("$", 1);
		if (!var_name)
			return (1);
		addback_token(lst, create_token(var_name, WORD));
	}
	else
	{
		var_name = ft_strndup(input + start, *i - start);
		if (!var_name)
			return (1);
		addback_token(lst, create_token(var_name, EXPAND));
	}
	return (1);
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

static int	extract_operator_token(size_t* i, const char* input, t_token** lst)
{
	char* op;

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
		return (1);
	addback_token(lst, create_token(op, det_op_type(op)));
	return (1);
}

int lexer(char* input, t_token **token_lst)
{
	size_t	i;
	int		error;

	*token_lst = NULL;
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (!input[i])
			break;
		if (input[i] == '\'' || input[i] == '"')
			error = extract_quoted_token(&i, input, token_lst);
		else if (input[i] == '$')
			error = extract_dollar_token(&i, input, token_lst);
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>' \
			|| input[i] == '&')
			error = extract_operator_token(&i, input, token_lst);
		else
			error = extract_word_token(&i, input, token_lst);
		if (!error)
		{
			printf("minishell: memory allocation failure.\n");
			return (0);
		}
	}
	print_tokens(token_lst);
	return (1);
}
