/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:29:39 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/30 13:18:08 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_segment	*create_segment(char *content, t_quote_type quote_type)
{
	t_token_segment	*segment;

	segment = malloc(sizeof(t_token_segment));
	if (!segment)
		return (NULL);
	segment->content = content;
	segment->quote_type = quote_type;
	segment->next = NULL;
	return (segment);
}

void	add_segment(t_token_segment **sgmnts, t_token_segment *n_sgmnt)
{
	t_token_segment	*current;

	if (!sgmnts || !n_sgmnt)
		return ;
	if (!*sgmnts)
	{
		*sgmnts = n_sgmnt;
		return ;
	}
	current = *sgmnts;
	while (current->next)
		current = current->next;
	current->next = n_sgmnt;
}

int	find_closing_quote(const char *input, char quote_char, size_t start)
{
	size_t	i;

	i = start + 1;
	while (input[i])
	{
		if (input[i] == quote_char)
			return (i);
		i++;
	}
	return (-1);
}

char	*extract_content(const char *input, size_t start, size_t end)
{
	size_t	len;

	len = end - start;
	if (len == 0)
		return (ft_strdup(""));
	return (ft_strndup(input + start, len));
}

int	create_default_segment(t_token *token, char *word)
{
	char			*content;
	t_token_segment	*segment;

	content = ft_strdup(word);
	if (!content)
		return (0);
	segment = create_segment(content, QUOTE_NONE);
	if (!segment)
	{
		free(content);
		return (0);
	}
	token->segments = segment;
	return (1);
}
