/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 16:52:48 by dmendoza          #+#    #+#             */
/*   Updated: 2025/10/03 16:52:49 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_arg_value(t_arg_token *arg)
{
	if (arg->expanded_value)
		return (arg->expanded_value);
	return (arg->original_token->value);
}

static int	count_args(t_arg_token *args)
{
	t_arg_token	*arg;
	int			count;

	count = 0;
	arg = args;
	while (arg)
	{
		count++;
		arg = arg->next;
	}
	return (count);
}

static void	free_argv_builder(char **argv, int up_to)
{
	int	i;

	i = 0;
	while (i < up_to)
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

static int	fill_argv(char **argv, t_arg_token *args)
{
	t_arg_token	*arg;
	int			i;

	arg = args;
	i = 0;
	while (arg)
	{
		argv[i] = ft_strdup(get_arg_value(arg));
		if (!argv[i])
		{
			free_argv_builder(argv, i);
			return (0);
		}
		arg = arg->next;
		i++;
	}
	argv[i] = NULL;
	return (1);
}

char	**get_argv_from_args(t_command *cmd)
{
	char	**argv;
	int		argc;

	if (!cmd || !cmd->args)
		return (NULL);
	argc = count_args(cmd->args);
	argv = malloc(sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL);
	if (!fill_argv(argv, cmd->args))
		return (NULL);
	return (argv);
}
