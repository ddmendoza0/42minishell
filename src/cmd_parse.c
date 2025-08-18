#include "minishell.h"

void	free_cmd_list(t_command *cmd)
{
    t_command *tmp;
    int i;

    while (cmd)
    {
        tmp = cmd->next;
        if (cmd->argv)
        {
            i = 0;
            while (cmd->argv[i])
            {
                free(cmd->argv[i]);
                i++;
            }
            free(cmd->argv);
        }
        if (cmd->input_file)
            free(cmd->input_file);
        if (cmd->output_file)
            free(cmd->output_file);
        if (cmd->subshell)
            free_cmd_list(cmd->subshell);
        free(cmd);
        cmd = tmp;
    }
}

int create_cmd(t_command **cmd)
{
    *cmd = malloc(sizeof(t_command));
    if (!*cmd)
        return 0;
    (*cmd)->argv = NULL;
    (*cmd)->input_file = NULL;
    (*cmd)->output_file = NULL;
    (*cmd)->append_output = 0;
    (*cmd)->heredoc = 0;
    (*cmd)->next = NULL;
    (*cmd)->subshell = NULL;
    (*cmd)->logic = CMD_NONE;
    return 1;
}

// Helper: extract tokens between LPAREN and matching RPAREN
static t_token *extract_subshell_tokens(t_token **current)
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

static int handle_lparen(t_command *cmd, t_token **current)
{
    t_token *sub_tokens;
    
    sub_tokens = extract_subshell_tokens(current);
    if (!sub_tokens)
    {
        printf("Syntax error: unmatched parenthesis\n");
        return 0;
    }
    cmd->subshell = cmd_builder(&sub_tokens);
    return 1;
}

static int	cmd_parse_tokens(t_command *cmd, t_token *current)
{
    t_command *new_cmd = NULL;
    while (current)
    {
        if (current->type == WORD)
        {
            if (!add_to_argv(cmd, current->value))
                return 0;
        }
        else if (current->type == REDIRECT_IN || current->type == HEREDOC)
        {
            if (!add_redir_in(cmd, current))
                return 0;
        }
        else if (current->type == REDIRECT_OUT || current->type == APPEND_OUT)
        {
            if (!add_redir_out(cmd, current))
                return 0;
        }
        else if (current->type == PIPE)
        {
            if (!create_cmd(&new_cmd))
                return 0;
            cmd->next = new_cmd;
            cmd = new_cmd;
        }
        else if (current->type == LPAREN)
        {
            if (!handle_lparen(cmd, &current))
                return 0;
            continue;
        }
        else if (current->type == AND)
        {
            cmd->logic = CMD_AND;
            if (!create_cmd(&new_cmd))
                return 0;
            cmd->next = new_cmd;
            cmd = new_cmd;
        }
        else if (current->type == OR)
        {
            cmd->logic = CMD_OR;
            if (!create_cmd(&new_cmd))
                return 0;
            cmd->next = new_cmd;
            cmd = new_cmd;
        }
        else
        {
            printf("Syntax error: unexpected token");
            if (current->value)
                printf(" '%s'", current->value);
            printf("\n");
            return 0;
        }
        current = current->next;
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