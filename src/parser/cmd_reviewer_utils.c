/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_reviewer_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:28:32 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/09 15:28:34 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*process_segment(t_token_segment *segment, t_shell *shell)
{
	if (!segment || !segment->content)
		return (ft_strdup(""));
	else if (segment->quote_type == QUOTE_SINGLE)
		return (ft_strdup(segment->content));
	else if (segment->quote_type == QUOTE_DOUBLE)
		return (expand_variables_in_string(segment->content, shell));
	else if (segment->quote_type == QUOTE_NONE)
		return (expand_variables_in_string(segment->content, shell));
	else
		return (ft_strdup(segment->content));
}

static char	*handle_fallback_token(t_token *token, t_shell *shell)
{
	if (token && token->value)
		return (expand_variables_in_string(token->value, shell));
	return (ft_strdup(""));
}

static char	*join_segments(char *result, char *new_segment)
{
	char	*temp;

	temp = ft_strjoin(result, new_segment);
	free(result);
	free(new_segment);
	return (temp);
}

char	*expand_token(t_token *token, t_shell *shell)
{
	t_token_segment	*current;
	char			*result;
	char			*processed_segment;

	if (!token || !token->segments)
		return (handle_fallback_token(token, shell));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	current = token->segments;
	while (current)
	{
		processed_segment = process_segment(current, shell);
		if (!processed_segment)
			return (free(result), NULL);
		result = join_segments(result, processed_segment);
		if (!result)
			return (NULL);
		current = current->next;
	}
	return (result);
}
