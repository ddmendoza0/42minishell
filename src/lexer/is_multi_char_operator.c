/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_multi_char_operator.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:58:20 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/11/12 16:58:29 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_multi_char_operator(const char *input)
{
	return (ft_strncmp(input, "&&", 2) == 0 || ft_strncmp(input, "||", 2)
		== 0 || ft_strncmp(input, ">>", 2) == 0
		|| ft_strncmp(input, "<<", 2) == 0);
}
