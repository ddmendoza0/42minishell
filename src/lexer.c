/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:46:37 by dmendoza          #+#    #+#             */
/*   Updated: 2025/07/11 20:01:40 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*last_token(t_token *token)
{
	t_token	*curr;

	curr = token;
	while (curr && curr->next)
		curr = curr->next;
	return (curr);
}

static t_token *create_token(void *value, t_token_type type)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = value;
	new->type = type;
	new->next = NULL;
	return (new);
}

static void addback_token(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!lst || !new)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		last = last_token(*lst);
		last->next = new;
	}
}

static t_toke_type detect_operat_type(const char *op)
{
	if (!op)
		return (WORD);
	if (ft_strcmp(op, "|") == 0)
		return (PIPE);
	else if (ft_strcmp(op, "<") == 0)
		return (REDIRECT_IN);
	else if (ft_strcmp(op, ">") == 0)
		return (REDIRECT_OUT);
	else if (ft_strcmp(op, ">>") == 0)
		return (APPEND_OUT);
	else if (ft_strcmp(op, "<<") == 0)
		return (HEREDOC);
}

void free_tokens(t_token *list)
{
	t_token *tmp;

	while (list)
	{
		tmp = list->next;
		free(list->value);
		free(list);
		list = tmp;
	}
}

static void ext_quoted_token(int *i, char *input, t_token **token_lst)
{
}

static void ext_operat_token(int *i, char *input, t_token **token_lst)
{
	char	*op;
	t_token	*new_token;

	op = NULL;
	if (input[*i] == '>' && input[*i + 1] == '>')
	{	op = strndup(">>", 2);
		*i += 2;
	}
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		op = strndup("<<", 2);
		*i += 2;
	}
	else
	{
		op = strndup(&input[*i], 1);
		(*i)++;
	}
	*new_token = create_token(op, detect_operator_type(op));
	token_addback(token_lst, new_token);
	free(op);
}

t_token	*lexer(char *input)
{
	int	i;
	t_token	*token_lst;

	token_lst = NULL;
	i = -1;
	while (input[++i])
	{
		if (input[i] == '\'' || input[i] == '"')
			//ext_quoted_token(&i, input, &token_list);
		else if (iniput[i] == '|' || input[i] == '<' || input[i] == '>')//se puede hacer un issoperator para readability.
			ext_operat_token(&i, input, &token_list);
		else
			//ext_word_token(&i, input, &token_list);
	}
}
