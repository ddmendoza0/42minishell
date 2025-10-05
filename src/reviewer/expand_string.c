/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:53:08 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:53:09 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_var_value(t_expand_data *data, char *var_value)
{
	size_t	k;

	k = 0;
	while (var_value[k] && data->j < data->result_size - 1)
		data->result[data->j++] = var_value[k++];
}

static void	process_variable(t_expand_data *data)
{
	char	*var_name;
	char	*var_value;

	data->i++;
	var_name = get_var_name(data->str, &data->i);
	if (var_name)
	{
		var_value = expand_variable(data->shell, var_name);
		if (var_value)
		{
			append_var_value(data, var_value);
			free(var_value);
		}
		free(var_name);
	}
}

static void	init_expand_data(t_expand_data *data, char *str, t_shell *shell)
{
	data->str = str;
	data->shell = shell;
	data->result_size = ft_strlen(str) * 256;
	data->result = malloc(data->result_size);
	data->i = 0;
	data->j = 0;
}

static int	should_expand(char *str, size_t i)
{
	if (str[i] != '$' || !str[i + 1])
		return (0);
	if (ft_isalnum(str[i + 1]) || str[i + 1] == '_'
		|| str[i + 1] == '?' || str[i + 1] == '$')
		return (1);
	return (0);
}

char	*expand_variables_in_string(char *str, t_shell *shell)
{
	t_expand_data	data;
	char			*final_result;

	if (!str || !shell)
		return (NULL);
	init_expand_data(&data, str, shell);
	if (!data.result)
		return (NULL);
	while (data.str[data.i] && data.j < data.result_size - 1)
	{
		if (should_expand(data.str, data.i))
			process_variable(&data);
		else
			data.result[data.j++] = data.str[data.i++];
	}
	data.result[data.j] = '\0';
	final_result = ft_strdup(data.result);
	free(data.result);
	return (final_result);
}
