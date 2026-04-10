/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   children_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 00:00:00 by peda-cos          #+#    #+#             */
/*   Updated: 2026/04/10 00:00:00 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	close_all_pipes(t_pipex *px)
{
	int	i;

	i = 0;
	while (i < px->cmd_count - 1)
	{
		close(px->pipes[i][0]);
		close(px->pipes[i][1]);
		i++;
	}
}

void	close_fds(t_pipex *px)
{
	close_all_pipes(px);
	close(px->fd_in);
	close(px->fd_out);
}

void	alloc_pipes(t_pipex *px)
{
	int	i;

	px->pipes = malloc(sizeof(int *) * (px->cmd_count - 1));
	if (!px->pipes)
		error_exit("malloc");
	i = 0;
	while (i < px->cmd_count - 1)
	{
		px->pipes[i] = malloc(sizeof(int) * 2);
		if (!px->pipes[i])
		{
			free_partial_pipes(px, i);
			error_exit("malloc");
		}
		if (pipe(px->pipes[i]) == -1)
		{
			free(px->pipes[i]);
			free_partial_pipes(px, i);
			error_exit("pipe");
		}
		i++;
	}
}

static void	setup_fds(t_pipex *px, int i)
{
	if (i == 0)
	{
		if (dup2(px->fd_in, STDIN_FILENO) == -1)
			error_exit("dup2");
	}
	else if (dup2(px->pipes[i - 1][0], STDIN_FILENO) == -1)
		error_exit("dup2");
	if (i == px->cmd_count - 1)
	{
		if (dup2(px->fd_out, STDOUT_FILENO) == -1)
			error_exit("dup2");
	}
	else if (dup2(px->pipes[i][1], STDOUT_FILENO) == -1)
		error_exit("dup2");
}

void	run_child(t_pipex *px, int i)
{
	setup_fds(px, i);
	close(px->fd_in);
	close(px->fd_out);
	close_all_pipes(px);
	execute_cmd(px->argv[px->cmd_start + i], px->envp);
}
