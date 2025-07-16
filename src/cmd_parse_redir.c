#include "minishell.h"

int add_redir_in(t_command *cmd, t_token *token)
{
    if (!cmd || !token)
        return 0;

    if (!token->next || token->next->type != WORD)
    {
        printf("Error: Expected a file name after redirection token.\n");
        return 0;
    }
    if (cmd->input_file)
        free(cmd->input_file);
    if (token->type == HEREDOC)
    {
        cmd->heredoc = 1;
        cmd->input_file = ft_strdup(token->next->value);
    }
    else if (token->type == REDIRECT_IN)
    {
        cmd->heredoc = 0;
        cmd->input_file = ft_strdup(token->next->value);
    }
    if (!cmd->input_file)
    {
        perror("Error allocating memory for input file name.\n");
        return 0;
    }
    return (1);
}

int add_redir_out(t_command *cmd, t_token *token)
{
    if (!cmd || !token)
        return 0;

    if (!token->next || token->next->type != WORD)
    {
        printf("Error: Expected a file name after redirection token.\n");
        return 0;
    }
    if (cmd->output_file)
        free(cmd->output_file);
    if (token->type == APPEND_OUT)
    {
        cmd->append_output = 1;
        cmd->output_file = ft_strdup(token->next->value);
    }
    else if (token->type == REDIRECT_OUT)
    {
        cmd->append_output = 0;
        cmd->output_file = ft_strdup(token->next->value);
    }
    if (!cmd->output_file)
    {
        perror("Error allocating memory for output file name.\n");
        return 0;
    }
    return (1);
}