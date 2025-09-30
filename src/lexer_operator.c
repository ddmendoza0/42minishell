/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:29:15 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/30 13:17:29 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_multi_char_operator(const char *input)
{
	return (ft_strncmp(input, "&&", 2) == 0
		|| ft_strncmp(input, "||", 2) == 0
		|| ft_strncmp(input, ">>", 2) == 0
		|| ft_strncmp(input, "<<", 2) == 0);
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
	else if (ft_strncmp(op, "(", 1) == 0)
		return (LPAREN);
	else if (ft_strncmp(op, ")", 1) == 0)
		return (RPAREN);
	else
		return (WORD);
}

static char	*create_str(size_t *i, const char *input, t_shell *s, t_token **lst)
{
	char	*op;

	if (is_multi_char_operator(&input[*i]))
	{
		op = ft_strndup(&input[*i], 2);
		*i += 2;
	}
	else
	{
		op = ft_strndup(&input[*i], 1);
		(*i)++;
	}
	if (!op)
	{
		handle_error(s, ERR_MALLOC, "operator token");
		free_token_lst(*lst);
		*lst = NULL;
	}
	return (op);
}

static t_token	*create_operator_token(char *op, t_shell *shell, t_token **lst)
{
	t_token	*token;

	token = create_token(op, det_op_type(op));
	if (!token)
	{
		handle_error(shell, ERR_MALLOC, "operator token");
		free(op);
		free_token_lst(*lst);
		*lst = NULL;
	}
	return (token);
}

int	extract_oprtr(size_t *i, const char *input, t_token **lst, t_shell *shell)
{
	char	*op;
	t_token	*token;

	op = create_str(i, input, shell, lst);
	if (!op)
		return (0);
	token = create_operator_token(op, shell, lst);
	if (!token)
		return (0);
	addback_token(lst, token);
	return (1);
}
