/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:30:21 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/09/30 16:07:40 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_arg_token	*create_arg_token(t_token *original)
{
	t_arg_token	*arg_token;

	if (!original)
		return (NULL);
	arg_token = malloc(sizeof(t_arg_token));
	if (!arg_token)
		return (NULL);
	arg_token->original_token = original;
	arg_token->expanded_value = NULL;
	arg_token->next = NULL;
	return (arg_token);
}

void	add_arg_token(t_arg_token **list, t_arg_token *new_arg)
{
	t_arg_token	*current;

	if (!list || !new_arg)
		return ;
	if (!*list)
	{
		*list = new_arg;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new_arg;
}

void	free_arg_tokens(t_arg_token *args)
{
	t_arg_token	*temp;

	while (args)
	{
		temp = args->next;
		if (args->expanded_value)
			free(args->expanded_value);
		free(args);
		args = temp;
	}
}

/*
 * REDIRECTION FILE MANAGEMENT - Clean Version
 */

t_redir_file	*create_redir_file(t_token *org, int append_mode, int is_hdoc)
{
	t_redir_file	*redir;

	if (!org)
		return (NULL);
	redir = malloc(sizeof(t_redir_file));
	if (!redir)
		return (NULL);
	redir->original_token = org;
	redir->expanded_path = NULL;
	redir->append_mode = append_mode;
	redir->is_heredoc = is_hdoc;
	redir->fd = -1;
	return (redir);
}

void	free_redir_file(t_redir_file *redir)
{
	size_t	path_len;

	if (!redir)
		return ;
	if (redir->fd >= 0)
		close(redir->fd);
	if (redir->expanded_path)
	{
		path_len = ft_strlen(redir->expanded_path);
		if ((ft_strncmp(redir->expanded_path, TMP_IN_FILE, path_len + 1) == 0) || (ft_strncmp(redir->expanded_path, TMP_OUT_FILE, path_len + 1) == 0) || (ft_strncmp(redir->expanded_path, TMP_HEREDOC_FILE, path_len + 1) == 0))
		{
			unlink(redir->expanded_path);
		}
		free(redir->expanded_path);
	}
	free(redir);
}

/*
 * PARSER INTEGRATION FUNCTIONS - Clean Version
 */

int	add_token_to_args(t_command *cmd, t_token *token)
{
	t_arg_token	*arg_token;

	if (!cmd || !token)
		return (0);
	arg_token = create_arg_token(token);
	if (!arg_token)
		return (0);
	add_arg_token(&cmd->args, arg_token);
	return (1);
}

static int	get_heredoc_mode(t_token *redir_token)
{
	if (redir_token->type == HEREDOC)
		return (1);
	return (0);
}

int	add_token_redir_in(t_command *cmd, t_token **token, t_shell *shell)
{
	t_token	*redir_token;
	t_token	*file_token;
	int		is_heredoc;

	if (!cmd || !token || !*token)
		return (0);
	redir_token = *token;
	file_token = redir_token->next;
	if (!file_token || file_token->type != WORD)
		return (handle_syntax_error(shell, "newline"));
	is_heredoc = get_heredoc_mode(redir_token);
	if (cmd->input_redir)
		free_redir_file(cmd->input_redir);
	cmd->input_redir = create_redir_file(file_token, 0, is_heredoc);
	if (!cmd->input_redir)
		return (0);
	*token = file_token->next;
	return (1);
}

static int	get_append_mode(t_token *redir_token)
{
	if (redir_token->type == APPEND_OUT)
		return (1);
	return (0);
}

int	add_token_redir_out(t_command *cmd, t_token **token, t_shell *shell)
{
	t_token	*redir_token;
	t_token	*file_token;
	int		append_mode;

	if (!cmd || !token || !*token)
		return (0);
	redir_token = *token;
	file_token = redir_token->next;
	if (!file_token || file_token->type != WORD)
		return (handle_syntax_error(shell, "newline"));
	append_mode = get_append_mode(redir_token);
	if (cmd->output_redir)
		free_redir_file(cmd->output_redir);
	cmd->output_redir = create_redir_file(file_token, append_mode, 0);
	if (!cmd->output_redir)
		return (0);
	*token = file_token->next;
	return (1);
}

static char	*get_var_name(char *str, size_t *i)
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
	data->result_size = ft_strlen(str) * 4;
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

static char	*get_arg_value(t_arg_token *arg)
{
	if (arg->expanded_value)
		return (arg->expanded_value);
	return (arg->original_token->value);
}

static int	count_args(t_arg_token *args)
{
	t_arg_token	*arg;
	int			count;

	count = 0;
	arg = args;
	while (arg)
	{
		count++;
		arg = arg->next;
	}
	return (count);
}

static void	free_argv(char **argv, int up_to)
{
	int	i;

	i = 0;
	while (i < up_to)
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static int	fill_argv(char **argv, t_arg_token *args)
{
	t_arg_token	*arg;
	int			i;

	arg = args;
	i = 0;
	while (arg)
	{
		argv[i] = ft_strdup(get_arg_value(arg));
		if (!argv[i])
		{
			free_argv(argv, i);
			return (0);
		}
		arg = arg->next;
		i++;
	}
	argv[i] = NULL;
	return (1);
}

char	**get_argv_from_args(t_command *cmd)
{
	char	**argv;
	int		argc;

	if (!cmd || !cmd->args)
		return (NULL);
	argc = count_args(cmd->args);
	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL);
	if (!fill_argv(argv, cmd->args))
		return (NULL);
	return (argv);
}

static char	*get_input_path(t_command *cmd)
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
}

int	is_heredoc_mode(t_command *cmd)
{
	if (!cmd || !cmd->input_redir)
		return (0);
	return (cmd->input_redir->is_heredoc);
}
