#include "minishell.h"

int handle_heredoc(t_redir_file *redir)
{
    int fd;
    char *line;
    size_t delimiter_len;

    fd = open(TMP_HEREDOC_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
    if (fd == -1)
    {
        perror("minishell: heredoc (tmp)");
        return 0;
    }
    delimiter_len = ft_strlen(redir->expanded_path);
    while (1)
    {
        ft_putstr_fd("> ", STDOUT_FILENO);
        line = readline(NULL);
        if (!line)
            break;
        if (ft_strncmp(line, redir->expanded_path, delimiter_len) == 0
            && line[delimiter_len] == '\0')
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    lseek(fd, 0, SEEK_SET); // rewind for reading
    redir->fd = fd;
    return 1;
}

int validate_input_redirection(t_redir_file *input_redir)
{
    if (input_redir)
    {
        if (input_redir->is_heredoc)
        {
            if (!input_redir->expanded_path || !*input_redir->expanded_path)
            {
                printf("minishell: heredoc: missing delimiter\n");
                return 0;
            }
            if (!handle_heredoc(input_redir))
                return 0;
        }
        else if (!input_redir->expanded_path || !*input_redir->expanded_path)
        {
            input_redir->fd = open(TMP_IN_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
            if (input_redir->fd == -1)
            {
                perror("minishell: input redirection (tmp)");
                return 0;
            }
        }
        else
        {
            input_redir->fd = open(input_redir->expanded_path, O_RDONLY);
            if (input_redir->fd == -1)
            {
                perror("minishell: input redirection");
                return 0;
            }
        }
    }
    return 1;
}

int validate_output_redirection(t_redir_file *output_redir)
{
    if (output_redir)
    {
        if (!output_redir->expanded_path || !*output_redir->expanded_path)
        {
            output_redir->fd = open(TMP_OUT_FILE, O_CREAT | O_RDWR | O_TRUNC, 0600);
            if (output_redir->fd == -1)
            {
                perror("minishell: output redirection (tmp)");
                return 0;
            }
        }
        else
        {
            if (output_redir->append_mode)
                output_redir->fd = open(output_redir->expanded_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
            else
                output_redir->fd = open(output_redir->expanded_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (output_redir->fd == -1)
            {
                perror("minishell: output redirection");
                return 0;
            }
        }
    }
    return 1;
}

int validate_command_redirections(t_command *cmd)
{
    if (!validate_input_redirection(cmd->input_redir))
        return 0;
    if (!validate_output_redirection(cmd->output_redir))
        return 0;
    if (cmd->subshell && !validate_command_redirections(cmd->subshell))
        return 0;
    if (cmd->next && !validate_command_redirections(cmd->next))
        return 0;
    return 1;
}