/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmendoza <dmendoza@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 18:46:37 by dmendoza          #+#    #+#             */
/*   Updated: 2025/07/15 18:31:32 by dmendoza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//testing
static const char* token_type_to_str(t_token_type type)
{
	if (type == WORD)
		return "WORD";
	if (type == PIPE)
		return "PIPE";
	if (type == REDIRECT_IN)
		return "REDIRECT_IN";
	if (type == REDIRECT_OUT)
		return "REDIRECT_OUT";
	if (type == APPEND_OUT)
		return "APPEND_OUT";
	if (type == HEREDOC)
		return "HEREDOC";
	if (type == AND)
		return "AND";
	if (type == OR)
		return "OR";
	if (type == LPAREN)
		return "LPAREN";
	if (type == RPAREN)
		return "RPAREN";
	if (type == EXPAND)
		return "EXPAND";
	return "UNKNOWN";
}

static void print_tokens_with_segments(t_token** list)
{
	int i = 0;
	int seg_num;
	const char* quote_names[] = { "NONE", "SINGLE", "DOUBLE" };
	t_token* curr = *list;
	t_token_segment* seg;

	while (curr)
	{
		printf("Token %d:\n", i);
		printf("  Type : %s\n", token_type_to_str(curr->type));
		printf("  Original Value: [%s]\n", curr->value ? (char*)curr->value : "NULL");

		if (curr->segments)
		{
			printf("  Segments:\n");
			seg = curr->segments;
			seg_num = 0;
			while (seg)
			{
				printf("    Segment %d: [%s] (%s)\n",
					seg_num, seg->content, quote_names[seg->quote_type]);
				seg = seg->next;
				seg_num++;
			}
		}
		else
			printf("  No segments\n");

		printf("\n");
		curr = curr->next;
		i++;
	}
}
//end testing

static int	is_single_char_operator(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&' || c == '(' || c == ')');
}

static int	is_multi_char_operator(const char* input)
{
	return (ft_strncmp(input, "&&", 2) == 0 || ft_strncmp(input, "||", 2) == 0 || ft_strncmp(input, ">>", 2) == 0 || ft_strncmp(input, "<<", 2) == 0);
}

static t_token_type	det_op_type(const char* op)
{
	if (ft_strncmp(op, "||", 2) == 0)
		return (OR);
	else if (ft_strncmp(op, "|", 1) == 0)
		return (PIPE);
	else if (ft_strncmp(op, ">>", 2) == 0)
		return (APPEND_OUT);
	else if (ft_strncmp(op, ">", 1) == 0)
		return (REDIRECT_OUT);
	else if (ft_strncmp(op, "<<", 2) == 0)
		return (HEREDOC);
	else if (ft_strncmp(op, "<", 1) == 0)
		return (REDIRECT_IN);
	else if (ft_strncmp(op, "&&", 2) == 0)
		return (AND);
	else if (ft_strncmp(op, "(", 1) == 0)
		return (LPAREN);
	else if (ft_strncmp(op, ")", 1) == 0)
		return (RPAREN);
	else
		return (WORD);
}

static t_token_segment* create_segment(char* content, t_quote_type quote_type)
{
	t_token_segment* segment;

	segment = malloc(sizeof(t_token_segment));
	if (!segment)
		return (NULL);
	segment->content = content;
	segment->quote_type = quote_type;
	segment->next = NULL;
	return (segment);
}

static void	add_segment(t_token_segment** segments,
	t_token_segment* new_segment)
{
	t_token_segment* current;

	if (!segments || !new_segment)
		return;
	if (!*segments)
	{
		*segments = new_segment;
		return;
	}
	current = *segments;
	while (current->next)
		current = current->next;
	current->next = new_segment;
}

static int	find_closing_quote(const char* input, char quote_char, size_t start)
{
	size_t	i;

	i = start + 1;
	while (input[i])
	{
		if (input[i] == quote_char)
			return (i);
		i++;
	}
	return (-1);
}

static char* extract_content(const char* input, size_t start, size_t end)
{
	size_t	len;

	len = end - start;
	if (len == 0)
		return (ft_strdup(""));
	return (ft_strndup(input + start, len));
}

static int	handle_quoted_section(size_t* i, const char* input,
	t_shell_state* shell, t_token** lst)
{
	char	quote_char;

	quote_char = input[*i];
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (input[*i] == quote_char)
		(*i)++;
	else
	{
		set_error(shell, ERR_UNCLOSED_QUOTE, "unclosed quote");
		free_token_lst(*lst);
		*lst = NULL;
		return (0);
	}
	return (1);
}

static int	find_word_end(size_t* i, const char* input,
	t_shell_state* shell, t_token** lst)
{
	while (input[*i] && !is_single_char_operator(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			if (!handle_quoted_section(i, input, shell, lst))
				return (0);
		}
		else if (ft_isspace(input[*i]))
			break;
		else
			(*i)++;
	}
	return (1);
}

static int	add_unquoted_segment(t_token* token, const char* word,
	size_t start, size_t end)
{
	char* content;
	t_token_segment* segment;

	if (end <= start)
		return (1);
	content = extract_content(word, start, end);
	if (!content)
		return (0);
	segment = create_segment(content, QUOTE_NONE);
	if (!segment)
	{
		free(content);
		return (0);
	}
	add_segment(&token->segments, segment);
	return (1);
}

static int	add_quoted_segment(t_token* token, const char* word,
	size_t start, int closing_pos)
{
	char* content;
	t_token_segment* segment;
	t_quote_type	current_quote;

	current_quote = (word[start] == '\'') ? QUOTE_SINGLE : QUOTE_DOUBLE;
	content = extract_content(word, start + 1, closing_pos);
	if (!content)
		return (0);
	segment = create_segment(content, current_quote);
	if (!segment)
	{
		free(content);
		return (0);
	}
	add_segment(&token->segments, segment);
	return (1);
}

static int	process_word_segments(t_token* token, char* word, t_shell_state* shell)
{
	size_t	j;
	size_t	word_len;
	size_t	segment_start;
	int		closing_pos;

	j = 0;
	word_len = ft_strlen(word);
	segment_start = 0;
	while (j < word_len)
	{
		if (word[j] == '\'' || word[j] == '"')
		{
			if (!add_unquoted_segment(token, word, segment_start, j))
				return (0);
			closing_pos = find_closing_quote(word, word[j], j);
			if (closing_pos == -1)
			{
				set_error(shell, ERR_UNCLOSED_QUOTE, "unclosed quote");
				return (0);
			}
			if (!add_quoted_segment(token, word, j, closing_pos))
				return (0);
			j = closing_pos + 1;
			segment_start = j;
		}
		else
			j++;
	}
	if (!add_unquoted_segment(token, word, segment_start, word_len))
		return (0);
	return (1);
}

static int	create_default_segment(t_token* token, char* word)
{
	char* content;
	t_token_segment* segment;

	content = ft_strdup(word);
	if (!content)
		return (0);
	segment = create_segment(content, QUOTE_NONE);
	if (!segment)
	{
		free(content);
		return (0);
	}
	token->segments = segment;
	return (1);
}

static int	extract_word_token(size_t* i, const char* input,
	t_token** lst, t_shell_state* shell)
{
	size_t	start;
	char* word;
	t_token* token;

	start = *i;
	if (!find_word_end(i, input, shell, lst))
		return (0);
	word = ft_strndup(input + start, *i - start);
	if (!word)
	{
		set_error(shell, ERR_MALLOC, "word token");
		free_token_lst(*lst);
		*lst = NULL;
		return (0);
	}
	token = create_token(word, WORD);
	if (!token)
	{
		set_error(shell, ERR_MALLOC, "word token");
		free(word);
		free_token_lst(*lst);
		*lst = NULL;
		return (0);
	}
	if (!process_word_segments(token, word, shell))
	{
		free_token_lst(*lst);
		*lst = NULL;
		return (0);
	}
	if (!token->segments)
	{
		if (!create_default_segment(token, word))
		{
			set_error(shell, ERR_MALLOC, "segment processing");
			free_token_lst(*lst);
			*lst = NULL;
			return (0);
		}
	}
	addback_token(lst, token);
	return (1);
}

static int	extract_operator_token(size_t* i, const char* input,
	t_token** lst, t_shell_state* shell)
{
	char* op;
	t_token* token;

	if (is_multi_char_operator(&input[*i]))
	{
		op = ft_strndup(&input[*i], 2);
		*i += 2;
	}
	else
	{
		op = ft_strndup(&input[*i], 1);
		(*i)++;
	}
	if (!op)
	{
		set_error(shell, ERR_MALLOC, "operator token");
		free_token_lst(*lst);
		*lst = NULL;
		return (0);
	}
	token = create_token(op, det_op_type(op));
	if (!token)
	{
		set_error(shell, ERR_MALLOC, "operator token");
		free(op);
		free_token_lst(*lst);
		*lst = NULL;
		return (0);
	}
	addback_token(lst, token);
	return (1);
}

int	lexer(char* input, t_token** token_lst, t_shell_state* shell)
{
	size_t	i;
	int		success;

	if (!input || !token_lst || !shell)
		return (0);
	*token_lst = NULL;
	clear_error(shell);
	i = 0;
	while (input[i])
	{
		while (ft_isspace(input[i]))
			i++;
		if (!input[i])
			break;
		if (is_single_char_operator(input[i]))
			success = extract_operator_token(&i, input, token_lst, shell);
		else
			success = extract_word_token(&i, input, token_lst, shell);
		if (!success)
			return (0);
	}
	print_tokens_with_segments(token_lst);
	return (1);
}
