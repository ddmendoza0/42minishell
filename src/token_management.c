#include "minishell.h"

/*
 * ARGUMENT TOKEN MANAGEMENT - Clean Version
 */

t_arg_token* create_arg_token(t_token* original)
{
    t_arg_token* arg_token;

    if (!original)
        return NULL;

    arg_token = malloc(sizeof(t_arg_token));
    if (!arg_token)
        return NULL;

    arg_token->original_token = original;
    arg_token->expanded_value = NULL;
    arg_token->next = NULL;

    return arg_token;
}

void add_arg_token(t_arg_token** list, t_arg_token* new_arg)
{
    t_arg_token* current;

    if (!list || !new_arg)
        return;

    if (!*list)
    {
        *list = new_arg;
        return;
    }

    current = *list;
    while (current->next)
        current = current->next;

    current->next = new_arg;
}

void free_arg_tokens(t_arg_token* args)
{
    t_arg_token* temp;

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

t_redir_file* create_redir_file(t_token* original, int append_mode, int is_heredoc)
{
    t_redir_file* redir;

    if (!original)
        return (NULL);
    redir = malloc(sizeof(t_redir_file));
    if (!redir)
        return (NULL);
    redir->original_token = original;
    redir->expanded_path = NULL;
    redir->append_mode = append_mode;
    redir->is_heredoc = is_heredoc;
    return (redir);
}

void free_redir_file(t_redir_file* redir)
{
    if (!redir)
        return ;
    if (redir->expanded_path)
        free(redir->expanded_path);
    free(redir);
}

/*
 * PARSER INTEGRATION FUNCTIONS - Clean Version
 */

int add_token_to_args(t_command* cmd, t_token* token)
{
    t_arg_token* arg_token;

    if (!cmd || !token)
        return (0);
    arg_token = create_arg_token(token);
    if (!arg_token)
        return (0);
    add_arg_token(&cmd->args, arg_token);
    return (1);
}

static int	get_heredoc_mode(t_token* redir_token)
{
    if (redir_token->type == HEREDOC)
        return (1);
    return (0);
}

int	add_token_redir_in(t_command* cmd, t_token** token)
{
    t_token* redir_token;
    t_token* file_token;
    int		is_heredoc;

    if (!cmd || !token || !*token)
        return (0);
    redir_token = *token;
    file_token = redir_token->next;
    if (!file_token || file_token->type != WORD)
    {
        printf("Error: Expected a file name after redirection token.\n");
        return (0);
    }
    is_heredoc = get_heredoc_mode(redir_token);
    if (cmd->input_redir)
        free_redir_file(cmd->input_redir);
    cmd->input_redir = create_redir_file(file_token, 0, is_heredoc);
    if (!cmd->input_redir)
        return (0);
    *token = file_token->next;
    return (1);
}

static int	get_append_mode(t_token* redir_token)
{
    if (redir_token->type == APPEND_OUT)
        return (1);
    return (0);
}

int	add_token_redir_out(t_command* cmd, t_token** token)
{
    t_token* redir_token;
    t_token* file_token;
    int		append_mode;

    if (!cmd || !token || !*token)
        return (0);
    redir_token = *token;
    file_token = redir_token->next;
    if (!file_token || file_token->type != WORD)
    {
        printf("Error: Expected a file name after redirection token.\n");
        return (0);
    }
    append_mode = get_append_mode(redir_token);
    if (cmd->output_redir)
        free_redir_file(cmd->output_redir);
    cmd->output_redir = create_redir_file(file_token, append_mode, 0);
    if (!cmd->output_redir)
        return (0);
    *token = file_token->next;
    return (1);
}

/*
 * VARIABLE EXPANSION FUNCTIONS - Clean Version
 */

char* expand_variables_in_string(char* str, t_shell* shell)
{
    char* result;
    char* var_name;
    char* var_value;
    size_t i, j, var_start;
    size_t result_size;

    if (!str || !shell)
        return (NULL);
    result_size = ft_strlen(str) * 4; // Generous buffer
    result = malloc(result_size);
    if (!result)
        return (NULL);
    i = 0;
    j = 0;
    while (str[i] && j < result_size - 1)
    {
        if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?' || str[i + 1] == '$'))
        {
            i++; // Skip $
            var_start = i;
            if (str[i] == '?' || str[i] == '$')
            {
                i++;
                var_name = ft_strndup(&str[var_start], 1);
            }
            else
            {
                while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                    i++;
                var_name = ft_strndup(&str[var_start], i - var_start);
            }
            if (var_name)
            {
                var_value = expand_variable(shell, var_name);
                if (var_value)
                {
                    size_t k = 0;
                    while (var_value[k] && j < result_size - 1)
                        result[j++] = var_value[k++];
                    free(var_value);
                }
                free(var_name);
            }
        }
        else
            result[j++] = str[i++];
    }
    result[j] = '\0';
    char* final_result = ft_strdup(result);
    free(result);
    return (final_result);
}

char* expand_from_segments(t_token_segment* segments, t_shell* shell)
{
    t_token_segment* current;
    char* result;
    char* temp;
    char* expanded_segment;

    if (!segments)
        return (ft_strdup(""));
    result = ft_strdup("");
    if (!result)
        return (NULL);
    current = segments;
    while (current)
    {
        if (current->quote_type == QUOTE_SINGLE)
            expanded_segment = ft_strdup(current->content);
        else
            expanded_segment = expand_variables_in_string(current->content, shell);
        if (!expanded_segment)
        {
            free(result);
            return (NULL);
        }
        temp = ft_strjoin(result, expanded_segment);
        free(result);
        free(expanded_segment);
        if (!temp)
            return (NULL);
        result = temp;
        current = current->next;
    }
    return (result);
}

static char* get_arg_value(t_arg_token* arg)
{
    if (arg->expanded_value)
        return (arg->expanded_value);
    return (arg->original_token->value);
}

char** get_argv_from_args(t_command* cmd)
{
    t_arg_token* arg;
    char** argv;
    int			argc;
    int			i;

    if (!cmd || !cmd->args)
        return (NULL);
    argc = 0;
    arg = cmd->args;
    while (arg)
    {
        argc++;
        arg = arg->next;
    }
    argv = malloc(sizeof(char*) * (argc + 1));
    if (!argv)
        return (NULL);
    arg = cmd->args;
    i = 0;
    while (arg)
    {
        argv[i] = ft_strdup(get_arg_value(arg));
        if (!argv[i])
        {
            while (--i >= 0)
                free(argv[i]);
            free(argv);
            return (NULL);
        }
        arg = arg->next;
        i++;
    }
    argv[i] = NULL;
    return (argv);
}

static char* get_input_path(t_command* cmd)
{
    if (cmd->input_redir->expanded_path)
        return (cmd->input_redir->expanded_path);
    return (cmd->input_redir->original_token->value);
}

char* get_input_file(t_command* cmd)
{
    if (!cmd || !cmd->input_redir)
        return (NULL);
    return (ft_strdup(get_input_path(cmd)));
}

static char* get_output_path(t_command* cmd)
{
    if (cmd->output_redir->expanded_path)
        return (cmd->output_redir->expanded_path);
    return (cmd->output_redir->original_token->value);
}

char* get_output_file(t_command* cmd)
{
    if (!cmd || !cmd->output_redir)
        return (NULL);
    return (ft_strdup(get_output_path(cmd)));
}

int is_append_mode(t_command* cmd)
{
    if (!cmd || !cmd->output_redir)
        return (0);

    return (cmd->output_redir->append_mode);
}

int is_heredoc_mode(t_command* cmd)
{
    if (!cmd || !cmd->input_redir)
        return (0);

    return (cmd->input_redir->is_heredoc);
}