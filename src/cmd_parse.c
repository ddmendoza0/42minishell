#include "minishell.h"

//TESTING
void print_command_tree(t_command* cmd, int depth)
{
    int i;
    t_arg_token* arg;

    while (cmd)
    {
        for (i = 0; i < depth; i++)
            printf("  ");
        printf("Command:\n");

        // Print arguments
        if (cmd->args)
        {
            arg = cmd->args;
            i = 0;
            while (arg)
            {
                for (int j = 0; j < depth; j++)
                    printf("  ");
                printf("  arg[%d]: %s", i, arg->original_token->value);
                if (arg->expanded_value)
                    printf(" (expanded: %s)", arg->expanded_value);
                printf("\n");
                arg = arg->next;
                i++;
            }
        }

        // Print input redirection
        if (cmd->input_redir)
        {
            for (i = 0; i < depth; i++)
                printf("  ");
            printf("  input: %s", cmd->input_redir->original_token->value);
            if (cmd->input_redir->expanded_path)
                printf(" (expanded: %s)", cmd->input_redir->expanded_path);
            if (cmd->input_redir->is_heredoc)
                printf(" (heredoc)");
            printf("\n");
        }

        // Print output redirection
        if (cmd->output_redir)
        {
            for (i = 0; i < depth; i++)
                printf("  ");
            printf("  output: %s", cmd->output_redir->original_token->value);
            if (cmd->output_redir->expanded_path)
                printf(" (expanded: %s)", cmd->output_redir->expanded_path);
            if (cmd->output_redir->append_mode)
                printf(" (append)");
            printf("\n");
        }

        // Print logic
        for (i = 0; i < depth; i++)
            printf("  ");
        printf("  logic: %s\n",
            cmd->logic == CMD_PIPE ? "PIPE" :
            cmd->logic == CMD_AND ? "AND" :
            cmd->logic == CMD_OR ? "OR" : "NONE");

        // Print subshell
        if (cmd->subshell)
        {
            for (i = 0; i < depth; i++)
                printf("  ");
            printf("  subshell:\n");
            print_command_tree(cmd->subshell, depth + 1);
        }

        cmd = cmd->next;
    }
}
// END TESTING
void free_cmd_list(t_command* cmd)
{
    t_command* tmp;

    while (cmd)
    {
        tmp = cmd->next;
        // Free arguments
        if (cmd->args)
            free_arg_tokens(cmd->args);
        // Free redirections
        if (cmd->input_redir)
            free_redir_file(cmd->input_redir);
        if (cmd->output_redir)
            free_redir_file(cmd->output_redir);
        // Free subshell
        if (cmd->subshell)
            free_cmd_list(cmd->subshell);
        free(cmd);
        cmd = tmp;
    }
}

static int create_cmd(t_command** cmd)
{
    *cmd = malloc(sizeof(t_command));
    if (!*cmd)
        return (0);
    (*cmd)->args = NULL;
    (*cmd)->input_redir = NULL;
    (*cmd)->output_redir = NULL;
    (*cmd)->logic = CMD_NONE;
    (*cmd)->subshell = NULL;
    (*cmd)->next = NULL;
    return (1);
}

// Helper: extract tokens between LPAREN and matching RPAREN
t_token *extract_subshell_tokens(t_token **current)
{
    t_token *start;
    t_token *end;
    t_token *after;
    int paren_count;

    start = (*current)->next;
    end = start;
    paren_count = 1;
    while (end && paren_count > 0)
    {
        if (end->type == LPAREN)
            paren_count++;
        else if (end->type == RPAREN)
            paren_count--;
        if (paren_count > 0)
            end = end->next;
    }
    if (paren_count != 0)
        return NULL; // Unmatched parenthesis
    if (end)
    {
        after = end->next;
        end->next = NULL;
    }
    else
        after = NULL;
    *current = after;
    return start;
}

int handle_lparen(t_command *cmd, t_token **current)
{
    t_token *sub_tokens;
    
    sub_tokens = extract_subshell_tokens(current);
    if (!sub_tokens)
    {
        printf("Syntax error: unmatched parenthesis\n");
        return 0;
    }   
    cmd->subshell = cmd_builder(&sub_tokens);
    return (cmd->subshell != NULL);
}

static int cmd_parse_tokens(t_command* cmd, t_token* current)
{
    t_command* new_cmd;

    while (current)
    {
        if (current->type == WORD)
        {
            if (!add_token_to_args(cmd, current))
                return 0;
            current = current->next;
        }
        else if (current->type == REDIRECT_IN || current->type == HEREDOC)
        {
            if (!add_token_redir_in(cmd, &current))
                return 0;
        }
        else if (current->type == REDIRECT_OUT || current->type == APPEND_OUT)
        {
            if (!add_token_redir_out(cmd, &current))
                return 0;
        }
        else if (current->type == PIPE || current->type == AND || current->type == OR)
        {
            if (current->type == AND)
                cmd->logic = CMD_AND;
            else if (current->type == OR)
                cmd->logic = CMD_OR;
            else if (current->type == PIPE)
                cmd->logic = CMD_PIPE;
            if (!create_cmd(&new_cmd))
                return 0;
            cmd->next = new_cmd;
            cmd = new_cmd;
            current = current->next;
        }
        else if (current->type == LPAREN)
        {
            if (!handle_lparen(cmd, &current))
                return 0;
        }
        else if (current->type == RPAREN)
            current = current->next;
        else
        {
            printf("Syntax error: unexpected token");
            if (current->value)
                printf(" '%s'", current->value);
            printf("\n");
            return 0;
        }
    }
    return 1;
}

t_command *cmd_builder(t_token **tkn_list)
{
    t_command *cmd;
    t_command *head;
    t_token *current;

    if (!tkn_list || !*tkn_list)
        return NULL;
    current = *tkn_list;
    if (!create_cmd(&cmd))
        return NULL;
    head = cmd;
    if (!cmd_parse_tokens(cmd, current))
    {
        free_cmd_list(head);
        return NULL;
    }
    return head;
}