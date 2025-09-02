/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:52:38 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/07/10 16:58:44 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// command 'history'
void     cmd_history(void)
{
        int     hs_fd;
        char    *line;
        size_t  len;

        hs_fd = open(HISTORY_FILE, O_RDONLY);
        while (1)
        {
                line = get_next_line(hs_fd);
                if (!line)
                        break ;
                len = ft_strlen(line);
                if (line [len - 1] == '\n')
                        line[len - 1] = '\0';
                ft_printf("%s\n", line);
                free(line);
        }
        close(hs_fd);
}

//command !* and !$ (expande estos argumentos, return NULL si no es posible expandir)
static char *join_args(char **args, int argc)
{
    int i;
    size_t total_len;
    char *result;
    char *ptr;

    total_len = 0;
    i = 0;
    while (i < argc)
    {
        total_len += ft_strlen(args[i]);
        if (i < argc - 1)
            total_len += 1;
        i++;
    }
    result = malloc(total_len + 1);
    if (!result)
        return NULL;
    ptr = result;
    i = 0;
    while (i < argc)
    {
        size_t len = ft_strlen(args[i]);
        ft_memcpy(ptr, args[i], len);
        ptr += len;
        if (i < argc - 1)
            *ptr++ = ' ';
        i++;
    }
    *ptr = '\0';
    return result;
}


char *expand_history_substitution(const char *pattern)
{
    HIST_ENTRY *last;
    char **args;
    int argc;
    int i;
    char *result;

    last = history_get(history_length - 1);
    if (!last)
        return NULL;
    args = ft_split(last->line, ' ');
    argc = 0;
    while (args && args[argc])
        argc++;
    result = NULL;
    if (ft_strcmp(pattern, "!$") == 0 && argc > 1)
        result = strdup(args[argc - 1]);
    else if (ft_strcmp(pattern, "!*") == 0 && argc > 1)
        result = join_args(args + 1, argc - 1);
    i = 0;
    while (args && args[i])
        free(args[i++]);
    free(args);
    return result;
}

//command 'fc n' (fix this command) habra que insertar el retorno en la linea de dialogo desde fuera
char *fc_command(int n)
{
    HIST_ENTRY *entry;

    if (n < 1 || n > history_length)
        return NULL;
    entry = history_get(n);
    if (!entry || !entry->line)
        return NULL;
    return strdup(entry->line);
}

//command 'history -d n'
void history_delete(int n)
{
    int fd, out_fd, idx = 1;
    char *line;

    if (n < 1 || n > history_length)
        return;
    remove_history(n - 1);
    fd = open(HISTORY_FILE, O_RDONLY);
    out_fd = open(HISTORY_FILE ".tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1 || out_fd == -1)
    {
        if (fd != -1) close(fd);
        if (out_fd != -1) close(out_fd);
        return;
    }
    while ((line = get_next_line(fd)))
    {
        if (idx != n)
            write(out_fd, line, ft_strlen(line));
        free(line);
        idx++;
    }
    close(fd);
    close(out_fd);
    rename(HISTORY_FILE ".tmp", HISTORY_FILE);
}

//command 'history -c'
void history_clear(void)
{
    clear_history();
    int fd = open(HISTORY_FILE, O_WRONLY | O_TRUNC);
    if (fd != -1)
        close(fd);
}

//command '!!' expande al ultimo comando en el historial
char *expand_history_bangbang(void)
{
    HIST_ENTRY *last;

    last = history_get(history_length - 1);
    if (!last || !last->line)
        return NULL;
    return strdup(last->line);
}

//command '!n' expande al comando n en el historial
char *expand_history_bang_n(int n)
{
    HIST_ENTRY *entry;

    if (n < 1 || n > history_length)
        return NULL;
    entry = history_get(n);
    if (!entry || !entry->line)
        return NULL;
    return strdup(entry->line);
}

// command '!-n' expande al comando n lineas antes del actual
char *expand_history_bang_minus_n(int n)
{
    int idx;
    HIST_ENTRY *entry;

    if (n < 1 || n >= history_length)
        return NULL;
    idx = history_length - n;
    entry = history_get(idx);
    if (!entry || !entry->line)
        return NULL;
    return strdup(entry->line);
}

// command '!string' expande al primerr comando que comienza con string
char *expand_history_bang_string(const char *str)
{
    int i;
    HIST_ENTRY *entry;
    size_t len;

    if (!str || !*str)
        return NULL;
    len = ft_strlen(str);
    i = history_length;
    while (i > 0)
    {
        entry = history_get(i);
        if (entry && entry->line && ft_strncmp(entry->line, str, len) == 0)
            return strdup(entry->line);
        i--;
    }
    return NULL;
}
