/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 00:00:00 by peda-cos          #+#    #+#             */
/*   Updated: 2026/04/10 00:00:00 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_one(t_pipex *px)
{
	if (dup2(px->fd_in, STDIN_FILENO) == -1)
		error_exit("dup2");
	close(px->fd_in);
	if (dup2(px->pipe_fd[1], STDOUT_FILENO) == -1)
		error_exit("dup2");
	close(px->pipe_fd[0]);
	close(px->pipe_fd[1]);
	close(px->fd_out);
	execute_cmd(px->argv[2], px->envp);
}

void	child_two(t_pipex *px)
{
	if (dup2(px->pipe_fd[0], STDIN_FILENO) == -1)
		error_exit("dup2");
	close(px->pipe_fd[1]);
	close(px->pipe_fd[0]);
	if (dup2(px->fd_out, STDOUT_FILENO) == -1)
		error_exit("dup2");
	close(px->fd_out);
	close(px->fd_in);
	execute_cmd(px->argv[3], px->envp);
}
