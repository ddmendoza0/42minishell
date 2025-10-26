/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmaya-vi <dmaya-vi@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:25:51 by dmaya-vi          #+#    #+#             */
/*   Updated: 2025/10/26 13:17:16 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_full_q(t_token *token)
{
	t_token_segment	*seg;

	if (!token || !token->segments)
		return (0);
	seg = token->segments;
	if (seg->next == NULL && seg->quote_type != QUOTE_NONE)
		return (1);
	return (0);
}

static int	check_n_flag(char *arg, t_arg_token *arg_t)
{
	int	j;

	if (arg_t && is_full_q(arg_t->original_token))
		return (0);
	if (!arg || arg[0] != '-' || !arg[1])
		return (0);
	j = 1;
	while (arg[j] == 'n')
		j++;
	if (arg[j] == '\0')
		return (1);
	return (0);
}

int	builtin_echo(char **argv, t_command *cmd, t_shell *shell)
{
	int			i;
	int			newline;
	t_arg_token	*arg_t;

	i = 1;
	newline = 1;
	arg_t = cmd->args->next;
	while (argv[i] && arg_t && check_n_flag(argv[i], arg_t))
	{
		newline = 0;
		i++;
		arg_t = arg_t->next;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (set_exit_status(shell, EXIT_SUCCESS));
}
