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

/*Comando "history" imprime todo el historial con el numero de linea al principio*/
void     cmd_history(void)
{
        int     hs_fd;
        char    *line;
        size_t  len;
        unsigned int i = 1;

        hs_fd = open(HISTORY_FILE, O_RDONLY);
        while (1)
        {
                line = get_next_line(hs_fd);
                if (!line)
                        break ;
                len = ft_strlen(line);
                if (len > 0 && line[len - 1] == '\n')
                    line[len - 1] = '\0';
                printf("%u\t%s\n", i, line);
                free(line);
                i++;
        }
        close(hs_fd);
}

//Comando "history -c y history -d n" el input deberia ser los flags del comando
//el input se pùede cambiar por (char *flag, int n) o algo parecido
//Work in progress
void cmd_delete_history(char *input)
{
	int n;
	int hs_fd;
        char *line;

	if (hs_fd == -1)
	{
		perror("Error accessing the command history.\n");
		return;
	}
	while (*input) //cada if se puede meter en otras funciones
	{
                if (*input == 'c')
                {
                        hs_fd = open(HISTORY_FILE, O_TRUNC | O_WRONLY);
                        if (hs_fd == -1)
                        {
                                perror("Error clearing the command history.\n");
                                return;
                        }
                        rl_clear_history();
                }
                else if (*input == 'd')
                {
                        input += 2;
                        n = atoi(input);
                        if (n > 0)
                        {
                                //Funcion para recrear el archivo sin la linea n aqui
                            	hs_fd = open(HISTORY_FILE, O_RDWR);
                                rl_clear_history();
                                initialize_history();
                        }
                }
	}
}

/*Comando "fc n" que introduce la entrada de historial n en la linea de dialogo 
(necesita parseo para poder testearlo)*/
//WIP
char    *cmd_fix_cmd(char *input)
{
        int     i;
        int     n;
        int     hs_fd;
        char    *line = NULL;

        n = atoi(input);
        if (n <= 0)
                return (NULL);
        hs_fd = open(HISTORY_FILE, O_RDONLY);
        if (hs_fd == -1)
        {
                perror("Error accessing the command history.\n");
                return (NULL);
        }
        i = 1;
        while (i <= n)
        {
                line = get_next_line(hs_fd);
                if (!line)
                        break;
                if (i != n)
                        free(line);
                i++;
        }
        close(hs_fd);
        if (line && i - 1 == n)
                return (line);
        if (line)
                free(line);
        return (NULL);
}