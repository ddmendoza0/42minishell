#include "minishell.h"

int add_redir_in(t_command *cmd, t_token **token)
{
    if (!cmd || !*token)
        return 0;

    t_token *redir = *token;
    t_token *file_token = redir->next;

    if (!file_token || file_token->type != WORD)
    {
        printf("Error: Expected a file name after redirection token.\n");
        return 0;
    }
    if (cmd->input_file)
        free(cmd->input_file);
    if (redir->type == HEREDOC)
    {
        cmd->heredoc = 1;
        cmd->input_file = ft_strdup(file_token->value);
    }
    else if (redir->type == REDIRECT_IN)
    {
        cmd->heredoc = 0;
        cmd->input_file = ft_strdup(file_token->value);
    }
    if (!cmd->input_file)
    {
        perror("Error allocating memory for input file name.\n");
        return 0;
    }
    *token = file_token->next; // Advance token pointer past file name
    return 1;
}

int add_redir_out(t_command *cmd, t_token **token)
{
    if (!cmd || !*token)
        return 0;

    t_token *redir = *token;
    t_token *file_token = redir->next;

    if (!file_token || file_token->type != WORD)
    {
        printf("Error: Expected a file name after redirection token.\n");
        return 0;
    }
    if (cmd->output_file)
        free(cmd->output_file);
    if (redir->type == APPEND_OUT)
    {
        cmd->append_output = 1;
        cmd->output_file = ft_strdup(file_token->value);
    }
    else if (redir->type == REDIRECT_OUT)
    {
        cmd->append_output = 0;
        cmd->output_file = ft_strdup(file_token->value);
    }
    if (!cmd->output_file)
    {
        perror("Error allocating memory for output file name.\n");
        return 0;
    }
    *token = file_token->next; // Advance token pointer past file name
    return 1;
}