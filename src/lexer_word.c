/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:29:50 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/09 15:29:53 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int	handle_q_sctn(size_t *i, const char *input, t_shell *shell, t_token **lst)
{
	char	quote_char;

	quote_char = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (input[*i] == quote_char)
		(*i)++;
	else
	{
		handle_error(shell, ERR_UNCLOSED_QUOTE, "unclosed quote");
		free_token_lst(*lst);
		*lst = NULL;
		return (0);
	}
	return (1);
}

static int	find_w_end(size_t *i, const char *input, t_shell *s, t_token **lst)
{
	while (input[*i] && !is_single_char_operator(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			if (!handle_q_sctn(i, input, s, lst))
				return (0);
		}
		else if (ft_isspace(input[*i]))
			break ;
		else
			(*i)++;
	}
	return (1);
}

static char	*crt_w_str(const char *str, size_t len, t_shell *s, t_token **lst)
{
	char	*word;

	word = ft_strndup(str, len);
	if (!word)
	{
		handle_error(s, ERR_MALLOC, "word token");
		free_token_lst(*lst);
		*lst = NULL;
	}
	return (word);
}

static t_token	*create_word_token(char *word, t_shell *shell, t_token **lst)
{
	t_token	*token;

	token = create_token(word, WORD);
	if (!token)
	{
		handle_error(shell, ERR_MALLOC, "word token");
		free(word);
		free_token_lst(*lst);
		*lst = NULL;
	}
	return (token);
}

int	extract_word(size_t *i, const char *input, t_token **lst, t_shell *shell)
{
	size_t	start;
	char	*word;
	t_token	*token;

	start = *i;
	if (!find_w_end(i, input, shell, lst))
		return (0);
	word = crt_w_str(input + start, *i - start, shell, lst);
	if (!word)
		return (0);
	token = create_word_token(word, shell, lst);
	if (!token)
		return (0);
	if (!process_segments(token, word, shell, lst))
		return (0);
	addback_token(lst, token);
	return (1);
}
