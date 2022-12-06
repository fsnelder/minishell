/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   lexer_parse_functions.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: fsnelder <fsnelder@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/06 10:01:10 by fsnelder      #+#    #+#                 */
/*   Updated: 2022/12/06 10:07:27 by fsnelder      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "lexer_utils.h"
#include "util.h"
#include <stdio.h>

int	parse_quoted(char ch, t_lexer *lexer)
{
	char	quote;
	char	*end;

	quote = *lexer->line;
	end = ft_strchr(lexer->line + 1, quote);
	if (!end)
	{
		printf("expected closing quote: %c\n", ch);
		return (GENERAL_ERROR);
	}
	token_add_string(lexer, lexer->line, (end - lexer->line) + 1);
	lexer->line = end;
	return (SUCCESS);
}

int	parse_generic(char ch, t_lexer *lexer)
{
	token_add_string(lexer, lexer->line, 1);
	return (SUCCESS);
}

int	parse_redirect_in(char ch, t_lexer *lexer)
{
	if (lexer->current && lexer->current->type == REDIRECT_IN)
	{
		lexer->current->type = REDIRECT_HEREDOC;
		lexer->current->length += 1;
	}
	else
	{
		lexer_flush_token(lexer);
		lexer_new_token(lexer, REDIRECT_IN, lexer->line, 1);
	}
	return (SUCCESS);
}

int	parse_redirect_out(char ch, t_lexer *lexer)
{
	if (lexer->current && lexer->current->type == REDIRECT_OUT)
	{
		lexer->current->type = REDIRECT_APPEND;
		lexer->current->length += 1;
	}
	else
	{
		lexer_flush_token(lexer);
		lexer_new_token(lexer, REDIRECT_OUT, lexer->line, 1);
	}
	return (SUCCESS);
}

int	parse_pipe(char ch, t_lexer *lexer)
{
	lexer_flush_token(lexer);
	lexer_new_token(lexer, PIPE, lexer->line, 1);
	return (SUCCESS);
}
