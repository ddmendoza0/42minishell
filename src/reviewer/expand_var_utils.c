/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:53:37 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:53:38 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_var_name(char *str, size_t *i)
{
	size_t	var_start;
	char	*var_name;

	var_start = *i;
	if (str[*i] == '?' || str[*i] == '$')
	{
		(*i)++;
		var_name = ft_strndup(&str[var_start], 1);
	}
	else
	{
		while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
			(*i)++;
		var_name = ft_strndup(&str[var_start], *i - var_start);
	}
	return (var_name);
}
