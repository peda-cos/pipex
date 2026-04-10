/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 00:00:00 by peda-cos          #+#    #+#             */
/*   Updated: 2026/04/10 00:00:00 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	handle_heredoc(char *limiter, int *pipe_fd)
{
	char	*line;
	size_t	len;

	if (pipe(pipe_fd) == -1)
		error_exit("pipe");
	len = ft_strlen(limiter);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (ft_strncmp(line, limiter, len) == 0
			&& (line[len] == '\n' || line[len] == '\0'))
		{
			free(line);
			break ;
		}
		if (write(pipe_fd[1], line, ft_strlen(line)) == -1)
		{
			free(line);
			error_exit("write");
		}
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	close(pipe_fd[1]);
}
