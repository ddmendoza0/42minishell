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
        free(cmd);
        cmd = tmp;
    }
}

int create_cmd(t_command *cmd)
{
    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return 0;
    cmd->argv = NULL;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append_output = 0;
    cmd->heredoc = 0;
    cmd->next = NULL;
    return 1;
}

static int	cmd_parse_tokens(t_command *cmd, t_token *current)
{
    while (current)
    {
        if (current->type == WORD)
        {
            if (!add_to_argv(cmd, current->value))
                return 0;
        }
        //Gestionar que si hay mas de un redirect, se guarde solo el ultimo
        //sin inportar que el anterior sea invalido
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
        else if (current->type == PIPE || current->type == SEMICOLON)
        {
            if (!create_cmd(cmd->next))
            {
                //Funcion de liberar los comandos aqui
                return 0;
            }
            cmd = cmd->next;
        }
        else if (current->type == INVALID)
        {
            printf("Unexpected or invalid token: %s\n", current->value);
            //Funcion de liberar los comandos aqui
            return 0;
        }
        current = current->next;
    }
    current->next = NULL;
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
    if (!create_cmd(cmd))
        return NULL;
    head = cmd;
    current = *tkn_list;
    if (!cmd_parse_tokens(cmd, current))
        return NULL;
    return head;
}