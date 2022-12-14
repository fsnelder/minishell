/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: fsnelder <fsnelder@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/06 12:33:29 by fsnelder      #+#    #+#                 */
/*   Updated: 2022/12/12 13:57:33 by fsnelder      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "util.h"
#include "lexer.h"
#include "parser.h"
#include "parser_utils.h"
#include "libft.h"
#include "util.h"
#include <stdio.h>

static void	parser_init(t_parser *parser, t_list *tokens, t_list **commands)
{
	*commands = NULL;
	parser->command = NULL;
	parser->commands = commands;
	parser->token = tokens;
}

void	parser_init_command(t_parser *parser)
{
	if (parser->command != NULL)
		return ;
	parser->command = ft_malloc(1 * sizeof(t_command));
	parser->command->arguments = NULL;
	parser->command->redirections = NULL;
	parser->command->argv = NULL;
}

void	command_add_redirect(
		t_command *command, t_redirect_type type, t_token *word)
{
	t_redirect	*redirect;

	redirect = ft_malloc(1 * sizeof(t_redirect));
	redirect->redirect_type = type;
	redirect->word = word;
	redirect->expanded = NULL;
	ft_lstadd_back(&command->redirections, malloc_check(ft_lstnew(redirect)));
}

typedef int	(*t_token_parser)(t_parser *, t_token *);

static int	parse_node(t_parser *parser)
{
	static const t_token_parser	parsers[] = {
	[TOKEN_REDIRECT_APPEND] = parse_append,
	[TOKEN_REDIRECT_HEREDOC] = parse_heredoc,
	[TOKEN_REDIRECT_IN] = parse_in,
	[TOKEN_REDIRECT_OUT] = parse_out,
	[PIPE] = parse_pipe,
	[WORD] = parse_word
	};
	t_token						*token;

	token = (t_token *)parser->token->content;
	return ((parsers[token->type])(parser, token));
}

int	parse(t_list *tokens, t_list **commands)
{
	t_parser	parser;
	int			result;

	parser_init(&parser, tokens, commands);
	while (parser.token)
	{
		result = parse_node(&parser);
		if (result != SUCCESS)
		{
			parser_destroy(&parser);
			return (result);
		}
	}
	parser_flush_command(&parser);
	parser.commands = NULL;
	return (SUCCESS);
}
