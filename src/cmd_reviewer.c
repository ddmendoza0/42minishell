#include "minishell.h"

// Helper function to expand variables in a string
static char* expand_variables(char* str, t_shell* shell)
{
    char* result;
    char* var_name;
    char* var_value;
    char* expanded;
    int i, j, k, var_start;

    if (!str || !shell)
        return (NULL);

    result = malloc(ft_strlen(str) * 4); // Allocate for expansion
    if (!result)
        return (NULL);

    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?' || str[i + 1] == '$'))
        {
            i++; // Skip $
            var_start = i;

            // Handle special case $?
            if (str[i] == '?' || str[i] == '$')
            {
                i++;
                var_name = ft_strndup(&str[var_start], 1);
            }
            else
            {
                // Find end of variable name
                while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                    i++;

                // Extract variable name
                var_name = ft_strndup(&str[var_start], i - var_start);
            }

            if (!var_name)
            {
                free(result);
                return (NULL);
            }

            // Get variable value
            var_value = expand_variable(shell, var_name);
            if (var_value)
            {
                // Copy variable value to result
                k = 0;
                while (var_value[k])
                    result[j++] = var_value[k++];
                free(var_value);
            }

            free(var_name);
        }
        else
        {
            result[j++] = str[i++];
        }
    }
    result[j] = '\0';

    // Realloc to exact size
    expanded = ft_strdup(result);
    free(result);
    printf("DEBUG: Original: [%s]\n", str);
    printf("DEBUG: Expanded: [%s]\n", expanded);
    return (expanded);
}

// Review and process arguments in a command
static int review_command_args(t_command* cmd, t_shell* shell)
{
    int i;
    char* expanded;

    if (!cmd || !cmd->argv || !shell)
        return (1);

    i = 0;
    while (cmd->argv[i])
    {
        // Expand variables in each argument
        expanded = expand_variables(cmd->argv[i], shell);
        if (!expanded)
            return (0);

        // Replace the old argument with expanded one
        free(cmd->argv[i]);
        cmd->argv[i] = expanded;

        i++;
    }

    return (1);
}

// Review and process redirection files
static int review_redirections(t_command* cmd, t_shell* shell)
{
    char* expanded;

    if (!cmd || !shell)
        return (0);

    // Process input file
    if (cmd->input_file)
    {
        expanded = expand_variables(cmd->input_file, shell);
        if (!expanded)
            return (0);
        free(cmd->input_file);
        cmd->input_file = expanded;
    }

    // Process output file
    if (cmd->output_file)
    {
        expanded = expand_variables(cmd->output_file, shell);
        if (!expanded)
            return (0);
        free(cmd->output_file);
        cmd->output_file = expanded;
    }

    return (1);
}

// Validate command syntax
static int validate_command(t_command* cmd)
{
    if (!cmd)
        return (0);

    // Check if command has arguments or redirections
    if (!cmd->argv && !cmd->input_file && !cmd->output_file && !cmd->subshell)
    {
        printf("Error: empty command\n");
        return (0);
    }

    return (1);
}

// Main lexical review function for a single command
static int review_single_command(t_command* cmd, t_shell* shell)
{
    if (!cmd || !shell)
        return (0);

    // Process subshell recursively
    if (cmd->subshell)
    {
        if (!lexical_review(cmd->subshell, shell))
            return (0);
    }

    // Review arguments
    if (!review_command_args(cmd, shell))
        return (0);

    // Review redirections
    if (!review_redirections(cmd, shell))
        return (0);

    // Validate command
    if (!validate_command(cmd))
        return (0);

    return (1);
}

// Main lexical review function - processes entire command list
int lexical_review(t_command* cmd_list, t_shell* shell)
{
    t_command* current;

    if (!shell)
        return (0);

    current = cmd_list;
    while (current)
    {
        if (!review_single_command(current, shell))
            return (0);
        current = current->next;
    }

    return (1);
}