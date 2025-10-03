/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_list.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:58:16 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 17:58:37 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir_list	*create_redir_list_node(t_redir_file *redir)
{
	t_redir_list	*node;

	if (!redir)
		return (NULL);
	node = malloc(sizeof(t_redir_list));
	if (!node)
		return (NULL);
	node->redir = redir;
	node->next = NULL;
	return (node);
}

void	add_redir_to_list(t_redir_list **list, t_redir_file *redir)
{
	t_redir_list	*node;
	t_redir_list	*current;

	if (!list || !redir)
		return ;
	node = create_redir_list_node(redir);
	if (!node)
		return ;
	if (!*list)
	{
		*list = node;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = node;
}

void	free_redir_list(t_redir_list *list)
{
	t_redir_list	*temp;

	while (list)
	{
		temp = list->next;
		if (list->redir)
			free_redir_file(list->redir);
		free(list);
		list = temp;
	}
}

t_redir_file	*get_last_redir(t_redir_list *list)
{
	t_redir_list	*current;

	if (!list)
		return (NULL);
	current = list;
	while (current->next)
		current = current->next;
	return (current->redir);
}
