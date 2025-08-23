#include "minishell.h"

/*
 * SEGMENT PROCESSING
 * Processes individual segments based on quote type
 */
static char* process_segment(t_token_segment* segment, t_shell* shell)
{
    if (!segment || !segment->content)
        return (ft_strdup(""));
    else if (segment->quote_type == QUOTE_SINGLE)
        return (ft_strdup(segment->content));
	else if (segment->quote_type == QUOTE_DOUBLE)
        return (expand_variables_in_string(segment->content, shell));
	else if (segment->quote_type == QUOTE_NONE)
		return (expand_variables_in_string(segment->content, shell));
    else
		return (ft_strdup(segment->content));
}

/*
 * TOKEN EXPANSION
 * Expands a complete token using its segments
 */
static char* expand_token(t_token* token, t_shell* shell)
{
    t_token_segment* current;
    char* result;
    char* temp;
    char* processed_segment;

    if (!token || !token->segments)
    {
        // Fallback: use direct value if no segments
        if (token && token->value)
            return (expand_variables_in_string(token->value, shell));
        return (ft_strdup(""));
    }
    result = ft_strdup("");
    if (!result)
        return (NULL);
    current = token->segments;
    while (current)
    {
        processed_segment = process_segment(current, shell);
        if (!processed_segment)
        {
            free(result);
            return (NULL);
        }
        temp = ft_strjoin(result, processed_segment);
        free(result);
        free(processed_segment);
        if (!temp)
            return (NULL);
        result = temp;
        current = current->next;
    }
    return (result);
}

/*
 * COMMAND EXPANSION
 * Expands all arguments in a command
 */

static int expand_command_args(t_command* cmd, t_shell* shell)
{
    t_arg_token* current;

    if (!cmd || !shell)
        return (0);
    current = cmd->args;
    while (current)
    {
        if (!current->expanded_value && current->original_token)
        {
            current->expanded_value = expand_token(current->original_token, shell);
            if (!current->expanded_value)
                return (0);
        }
        current = current->next;
    }
    return (1);
}

/*
 * REDIRECTION EXPANSION
 * Expands redirection file paths
 */

static int expand_command_redirections(t_command* cmd, t_shell* shell)
{
    if (!cmd || !shell)
        return 0;
    // Expand input redirection
    if (cmd->input_redir && !cmd->input_redir->expanded_path && \
        cmd->input_redir->original_token)
    {
        cmd->input_redir->expanded_path = expand_token( \
            cmd->input_redir->original_token, shell);
        if (!cmd->input_redir->expanded_path)
            return (0);
    }

    // Expand output redirection
    if (cmd->output_redir && !cmd->output_redir->expanded_path &&\
        cmd->output_redir->original_token)
    {
        cmd->output_redir->expanded_path = expand_token( \
            cmd->output_redir->original_token, shell);
        if (!cmd->output_redir->expanded_path)
            return (0);
    }
    return (1);
}

/*
 * COMMAND VALIDATION
 * Validates command syntax after expansion
 */
static int validate_command(t_command* cmd)
{
    if (!cmd)
        return (0);
    // A command must have at least arguments, redirections, or subshell
    if (!cmd->args && !cmd->input_redir && !cmd->output_redir && !cmd->subshell)
    {
        printf("Error: empty command\n");
        return (0);
    }
    // Validate input redirection filename
    if (cmd->input_redir && cmd->input_redir->expanded_path)
    {
        if (ft_strlen(cmd->input_redir->expanded_path) == 0)
        {
            printf("Error: empty input redirection filename\n");
            return (0);
        }
    }
    // Validate output redirection filename
    if (cmd->output_redir && cmd->output_redir->expanded_path)
    {
        if (ft_strlen(cmd->output_redir->expanded_path) == 0)
        {
            printf("Error: empty output redirection filename\n");
            return (0);
        }
    }
    return (1);
}

/*
 * SINGLE COMMAND PROCESSING
 * Processes a single command: expansion and validation
 */
static int process_single_command(t_command* cmd, t_shell* shell)
{
    if (!cmd || !shell)
        return (0);
    // Process subshell recursively if exists
    if (cmd->subshell)
    {
        if (!lexical_review(cmd->subshell, shell))
            return (0);
    }
    // Expand arguments using segments
    if (!expand_command_args(cmd, shell))
        return (0);
    // Expand redirections using segments
    if (!expand_command_redirections(cmd, shell))
        return (0);
    // Validate command syntax
    if (!validate_command(cmd))
        return (0);
    return (1);
}

/*
 * MAIN LEXICAL REVIEW FUNCTION
 * Processes entire command list
 */
int lexical_review(t_command* cmd_list, t_shell* shell)
{
    t_command* current;

    if (!shell)
        return (0);
    current = cmd_list;
    while (current)
    {
        if (!process_single_command(current, shell))
            return (0);
        current = current->next;
    }
    return (1);
}