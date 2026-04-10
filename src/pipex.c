/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 00:00:00 by peda-cos          #+#    #+#             */
/*   Updated: 2026/04/10 00:00:00 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	open_infile(t_pipex *px, char *filename)
{
	px->fd_in = open(filename, O_RDONLY);
	if (px->fd_in == -1)
	{
		perror(filename);
		px->fd_in = open("/dev/null", O_RDONLY);
		if (px->fd_in == -1)
			error_exit("/dev/null");
	}
}

static void	open_outfile(t_pipex *px, char *filename)
{
	px->outfile_err = 0;
	px->fd_out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (px->fd_out == -1)
	{
		perror(filename);
		px->outfile_err = 1;
		px->fd_out = open("/dev/null", O_WRONLY);
		if (px->fd_out == -1)
			error_exit("/dev/null");
	}
}

static void	pipex(t_pipex *px)
{
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (pipe(px->pipe_fd) == -1)
		error_exit("pipe");
	pid1 = fork();
	if (pid1 == -1)
		error_exit("fork");
	if (pid1 == 0)
		child_one(px);
	pid2 = fork();
	if (pid2 == -1)
		error_exit("fork");
	if (pid2 == 0)
		child_two(px);
	close(px->pipe_fd[0]);
	close(px->pipe_fd[1]);
	close(px->fd_in);
	close(px->fd_out);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, &status, 0);
	if (px->outfile_err)
		exit(1);
	exit(WEXITSTATUS(status));
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	px;

	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
		return (1);
	}
	px.argv = argv;
	px.envp = envp;
	open_infile(&px, argv[1]);
	open_outfile(&px, argv[4]);
	pipex(&px);
	return (0);
}
