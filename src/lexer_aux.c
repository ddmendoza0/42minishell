/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_aux.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 17:01:46 by dmendoza          #+#    #+#             */
/*   Updated: 2025/07/15 18:21:57 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_segments(t_token_segment* segments)
{
	t_token_segment* temp;

	while (segments)
	{
		temp = segments->next;
		free(segments->content);
		free(segments);
		segments = temp;
	}
}

t_token* create_token(void* value, t_token_type type)
{
	t_token* new;

	new = (t_token*)malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = value;
	new->type = type;
	new->segments = NULL;
	new->next = NULL;
	return (new);
}

t_token* last_token(t_token* token)
{
	t_token* curr;

	curr = token;
	while (curr && curr->next)
		curr = curr->next;
	return (curr);
}

void	addback_token(t_token** lst, t_token* new)
{
	t_token* last;

	if (!lst || !new)
		return;
	if (!*lst)
		*lst = new;
	else
	{
		last = last_token(*lst);
		last->next = new;
	}
}

void	free_token_lst(t_token* lst)
{
	t_token* tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst->value);
		free_segments(lst->segments);
		free(lst);
		lst = tmp;
	}
}