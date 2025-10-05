/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_segments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:29:26 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/30 15:45:50 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_unq_sgmnt(t_token *tk, const char *w, size_t start, size_t end)
{
	char			*content;
	t_token_segment	*segment;

	if (end <= start)
		return (1);
	content = extract_content(w, start, end);
	if (!content)
		return (0);
	segment = create_segment(content, QUOTE_NONE);
	if (!segment)
	{
		free(content);
		return (0);
	}
	add_segment(&tk->segments, segment);
	return (1);
}

static int	add_q_sgmnt(t_token *tk, const char *w, size_t start, int cl_pos)
{
	char			*content;
	t_token_segment	*segment;
	t_quote_type	current_quote;

	if (w[start] == '\'')
		current_quote = QUOTE_SINGLE;
	else
		current_quote = QUOTE_DOUBLE;
	content = extract_content(w, start + 1, cl_pos);
	if (!content)
		return (0);
	segment = create_segment(content, current_quote);
	if (!segment)
	{
		free(content);
		return (0);
	}
	add_segment(&tk->segments, segment);
	return (1);
}

int	process_quoted(t_token *tk, char *w, t_shell *shell, t_segment_data *data)
{
	int	closing_pos;

	if (!add_unq_sgmnt(tk, w, data->segment_start, data->j))
		return (0);
	closing_pos = find_closing_quote(w, w[data->j], data->j);
	if (closing_pos == -1)
	{
		handle_error(shell, ERR_UNCLOSED_QUOTE, "unclosed quote");
		return (0);
	}
	if (!add_q_sgmnt(tk, w, data->j, closing_pos))
		return (0);
	data->j = closing_pos + 1;
	data->segment_start = data->j;
	return (1);
}

int	process_word_segments(t_token *token, char *word, t_shell *shell)
{
	t_segment_data	data;

	data.j = 0;
	data.word_len = ft_strlen(word);
	data.segment_start = 0;
	while (data.j < data.word_len)
	{
		if (word[data.j] == '\'' || word[data.j] == '"')
		{
			if (!process_quoted(token, word, shell, &data))
				return (0);
		}
		else
			data.j++;
	}
	if (!add_unq_sgmnt(token, word, data.segment_start, data.word_len))
		return (0);
	return (1);
}

int	process_segments(t_token *token, char *word, t_shell *shell, t_token **lst)
{
	if (!process_word_segments(token, word, shell))
	{
		if (*lst)
		{
			free_token_lst(*lst);
			*lst = NULL;
		}
		return (0);
	}
	if (!token->segments)
	{
		if (!create_default_segment(token, word))
		{
			handle_error(shell, ERR_MALLOC, "segment processing");
			if (*lst)
			{
				free_token_lst(*lst);
				*lst = NULL;
			}
			return (0);
		}
	}
	return (1);
}
