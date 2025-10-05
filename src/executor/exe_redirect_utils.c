/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_redirect_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:48:16 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:48:17 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/wait.h>

int	stp_redir(t_command *cmd, int *s_stdin, int *s_stdout, t_shell *sh)
{
	if (!save_standard_fds(s_stdin, s_stdout, sh))
		return (0);
	return (app_redir(cmd, *s_stdin, *s_stdout, sh));
}
