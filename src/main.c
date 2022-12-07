/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fsnelder <fsnelder@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 12:06:30 by fsnelder          #+#    #+#             */
/*   Updated: 2022/12/07 12:40:19 by fsnelder         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "util.h"
#include "parser.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include "libft.h"

#define PROMPT "minishell -> "

static void	reset_readline(void)
{
	rl_replace_line("", false);
	rl_on_new_line();
	rl_redisplay();
}

static void	sigint_handler(int sig)
{
	// TODO: handle exit code
	printf("\n");
	reset_readline();
}

static int	initialize_signal_handlers(void)
{
	signal(SIGINT, sigint_handler);
	// signal(SIGQUIT, SIG_IGN);
	return (0);
}

void	print_command(void *cptr);

static void	execute_line(const char *line)
{
	t_list	*tokens;
	t_list	*commands;
	int		result;

	result = lexical_analysis(line, &tokens);
	if (result != SUCCESS)
		return ;
	ft_lstiter(tokens, print_token);
	result = parse(tokens, &commands);
	if (result != SUCCESS)
	{
		ft_lstclear(&tokens, free);
		return ;
	}
	ft_lstiter(commands, print_command);
	// CommandLine* command = parse(tokens);
	// int exit_code = execute(command);
	ft_lstclear(&tokens, free);
}

int	main(int argc, char *argv[], char *environ[])
{
	char	*line;

	(void)argc;
	(void)argv;
	initialize_signal_handlers();
	while (true)
	{
		line = readline(PROMPT);
		if (!line)
			break ;
		else if (*line == '\0')
			continue ;
		add_history(line);
		execute_line(line);
		free(line);
	}
	return (0);
}
