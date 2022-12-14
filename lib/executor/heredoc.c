/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: fsnelder <fsnelder@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/12/08 13:55:14 by fsnelder      #+#    #+#                 */
/*   Updated: 2022/12/12 13:33:03 by fsnelder      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "util.h"
#include <stdio.h>
#include "expand.h"
#include "parser.h"
#include "executor_utils.h"
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <stdbool.h>

static char	*generate_tempfile_name(int id)
{
	const char	*prefix = "/tmp/.minishell_temp_file_";
	char		*number;
	char		*filename;

	number = malloc_check(ft_itoa(id));
	filename = malloc_check(ft_strjoin(prefix, number));
	free(number);
	return (filename);
}

static char	*next_tempfile_name(void)
{
	static int	uid = 0;
	int			i;
	char		*filename;

	i = 0;
	while (i < INT_MAX)
	{
		filename = generate_tempfile_name(uid);
		if (access(filename, F_OK) != 0)
			return (filename);
		free(filename);
		i++;
		uid++;
	}
	return (NULL);
}

static int	heredoc_read(
	int fd, t_redirect *redirect, char *delimiter, bool should_expand)
{
	char	*line;

	while (true)
	{
		line = readline("> ");
		if (!line)
		{
			free(delimiter);
			return (GENERAL_ERROR);
		}
		if (ft_strncmp(line, delimiter, UINT64_MAX) == 0)
		{
			free(line);
			break ;
		}
		if (should_expand)
			line = heredoc_expand(line);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(delimiter);
	return (SUCCESS);
}

static int	do_heredoc(t_redirect *redirect)
{
	char	*filename;
	int		fd;

	filename = next_tempfile_name();
	if (!filename)
		return (GENERAL_ERROR);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(filename);
		return (GENERAL_ERROR);
	}
	if (heredoc_read(
			fd, redirect, expand_delimiter(redirect->word),
			check_should_expand(redirect->word)) != SUCCESS)
	{
		free(filename);
		return (GENERAL_ERROR);
	}
	close(fd);
	redirect->expanded = filename;
	return (SUCCESS);
}

// process heredocs for all commands
int	process_heredocs(t_list *commands)
{
	t_list		*redirects;
	t_redirect	*redirect;

	while (commands)
	{
		redirects = ((t_command *)commands->content)->redirections;
		while (redirects)
		{
			redirect = ((t_redirect *)redirects->content);
			if (redirect->redirect_type == REDIRECT_HEREDOC)
			{
				if (do_heredoc(redirect) != SUCCESS)
					return (GENERAL_ERROR);
			}
			redirects = redirects->next;
		}
		commands = commands->next;
	}
	return (SUCCESS);
}
