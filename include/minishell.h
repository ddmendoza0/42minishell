/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 18:36:45 by dmendoza          #+#    #+#             */
/*   Updated: 2025/07/10 14:42:30 by dmaya-vi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/****************************************************************/
/*			LIBRARIES		 		*/
/****************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# include "libft.h"
#include <fcntl.h>
//for the readline function and functioning history
#include <readline/readline.h>
#include <readline/history.h>
/****************************************************************/
/*			END LIBRARIES				*/
/****************************************************************/

#define HISTORY_FILE ".minishell_history"


void	write_to_history_file(char *input, int history_fd);

#endif
