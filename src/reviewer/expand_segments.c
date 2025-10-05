/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_segments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:52:58 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:52:59 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_expanded_segment(t_token_segment *current, t_shell *shell)
{
	char	*expanded_segment;

	if (current->quote_type == QUOTE_SINGLE)
		expanded_segment = ft_strdup(current->content);
	else
		expanded_segment = expand_variables_in_string(current->content, shell);
	return (expanded_segment);
}

static char	*join_and_free(char *result, char *expanded_segment)
{
	char	*temp;

	temp = ft_strjoin(result, expanded_segment);
	free(result);
	free(expanded_segment);
	return (temp);
}

char	*expand_from_segments(t_token_segment *segments, t_shell *shell)
{
	t_token_segment	*current;
	char			*result;
	char			*expanded_segment;

	if (!segments)
		return (ft_strdup(""));
	result = ft_strdup("");
	if (!result)
		return (NULL);
	current = segments;
	while (current)
	{
		expanded_segment = get_expanded_segment(current, shell);
		if (!expanded_segment)
		{
			free(result);
			return (NULL);
		}
		result = join_and_free(result, expanded_segment);
		if (!result)
			return (NULL);
		current = current->next;
	}
	return (result);
}
