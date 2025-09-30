/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:46:37 by dmendoza          #+#    #+#             */
/*   Updated: 2025/09/30 15:22:43 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_single_char_operator(char c)
{
	return (c == '|' || c == '<' || c == '>'
		|| c == '&' || c == '(' || c == ')');
}

int	lexer(char *input, t_token **token_lst, t_shell *shell)
{
	size_t	i;
	int		success;

	if (!input || !token_lst || !shell)
		return (0);
	*token_lst = NULL;
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (!input[i])
			break ;
		if (is_single_char_operator(input[i]))
			success = extract_oprtr(&i, input, token_lst, shell);
		else
			success = extract_word(&i, input, token_lst, shell);
		if (!success)
			return (0);
	}
	return (1);
}
