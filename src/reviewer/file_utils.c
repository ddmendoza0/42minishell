/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:53:44 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 18:05:42 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*static char	*get_input_path(t_command *cmd)
{
	if (cmd->input_redir->expanded_path)
		return (cmd->input_redir->expanded_path);
	return (cmd->input_redir->original_token->value);
}

char	*get_input_file(t_command *cmd)
{
	if (!cmd || !cmd->input_redir)
		return (NULL);
	return (ft_strdup(get_input_path(cmd)));
}

static char	*get_output_path(t_command *cmd)
{
	if (cmd->output_redir->expanded_path)
		return (cmd->output_redir->expanded_path);
	return (cmd->output_redir->original_token->value);
}

char	*get_output_file(t_command *cmd)
{
	if (!cmd || !cmd->output_redir)
		return (NULL);
	return (ft_strdup(get_output_path(cmd)));
}

int	is_append_mode(t_command *cmd)
{
	if (!cmd || !cmd->output_redir)
		return (0);
	return (cmd->output_redir->append_mode);
}*/

static char	*get_input_path(t_redir_file *input_redir)
{
	if (input_redir->expanded_path)
		return (input_redir->expanded_path);
	return (input_redir->original_token->value);
}

char	*get_input_file(t_command *cmd)
{
	t_redir_file	*input;

	if (!cmd)
		return (NULL);
	input = get_last_redir(cmd->input_redirs);
	if (!input)
		return (NULL);
	return (ft_strdup(get_input_path(input)));
}

static char	*get_output_path(t_redir_file *output_redir)
{
	if (output_redir->expanded_path)
		return (output_redir->expanded_path);
	return (output_redir->original_token->value);
}

char	*get_output_file(t_command *cmd)
{
	t_redir_file	*output;

	if (!cmd)
		return (NULL);
	output = get_last_redir(cmd->output_redirs);
	if (!output)
		return (NULL);
	return (ft_strdup(get_output_path(output)));
}

int	is_append_mode(t_command *cmd)
{
	t_redir_file	*output;

	if (!cmd)
		return (0);
	output = get_last_redir(cmd->output_redirs);
	if (!output)
		return (0);
	return (output->append_mode);
}
