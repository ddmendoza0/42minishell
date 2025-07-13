/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:46:37 by dmendoza          #+#    #+#             */
/*   Updated: 2025/07/13 17:27:39 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
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
        return "UNKNOWN";
}

void print_tokens(t_token *list)
{
        int i = 0;

        while (list)
        {
                printf("Token %d:\n", i);
                printf("  Type : %s\n", token_type_to_str(list->type));
                printf("  Value: [%s]\n\n", list->value);
                list = list->next;
                i++;
        }
}
//end testing
*/
static void	ext_word_token(size_t *i, const char *input, t_token **lst)
{
	size_t	start;
	char	*word;
	char	quote;

	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t' && \
					input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			quote = input[*i];
			(*i)++;
			while (input[*i] && input[*i] != quote)
				(*i)++;
			if (input[*i])
				(*i)++;
		}
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
	char	quote;
	size_t	start;
	char	*value;

	quote = input[*i];
	start = ++(*i);
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] != quote)
	{
		free(ft_strndup(input + start, *i - start));
		return ;
	}
	value = ft_strndup(input + start, *i - start);
	(*i)++;
	if (!value)
		return ;
	addback_token(lst, create_token(value, WORD));
}

static t_token_type	det_op_type(const char *op)
{
	if (ft_strncmp(op, "|", 1) == 0)
		return (PIPE);
	else if (ft_strncmp(op, ">>", 2) == 0)
		return (APPEND_OUT);
	else if (ft_strncmp(op, ">", 1) == 0)
		return (REDIRECT_OUT);
	else if (ft_strncmp(op, "<<", 2) == 0)
		return (HEREDOC);
	else if (ft_strncmp(op, "<", 1) == 0)
		return (REDIRECT_IN);
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
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			ext_operator_token(&i, input, &token_lst);
		else
			ext_word_token(&i, input, &token_lst);
	}
	//print_tokens(token_lst);
	return (token_lst);
}
