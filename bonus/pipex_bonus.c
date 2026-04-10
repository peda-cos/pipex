/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 00:00:00 by peda-cos          #+#    #+#             */
/*   Updated: 2026/04/10 00:00:00 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	open_infile(t_pipex *px, char **argv)
{
	int	hd_pipe[2];

	if (px->here_doc)
	{
		handle_heredoc(argv[2], hd_pipe);
		px->fd_in = hd_pipe[0];
	}
	else
	{
		px->fd_in = open(argv[1], O_RDONLY);
		if (px->fd_in == -1)
		{
			perror(argv[1]);
			px->fd_in = open("/dev/null", O_RDONLY);
			if (px->fd_in == -1)
				error_exit("/dev/null");
		}
	}
}

static void	open_outfile(t_pipex *px, char **argv, int argc)
{
	int	flags;

	if (px->here_doc)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	px->outfile_err = 0;
	px->fd_out = open(argv[argc - 1], flags, 0644);
	if (px->fd_out == -1)
	{
		perror(argv[argc - 1]);
		px->outfile_err = 1;
		px->fd_out = open("/dev/null", O_WRONLY);
		if (px->fd_out == -1)
			error_exit("/dev/null");
	}
}

static void	wait_and_exit(t_pipex *px, pid_t *pids)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < px->cmd_count)
		waitpid(pids[i++], &status, 0);
	free_pipes(px);
	free(pids);
	if (px->outfile_err)
		exit(1);
	exit(WEXITSTATUS(status));
}

static void	run_pipex(t_pipex *px)
{
	pid_t	*pids;
	int		i;

	alloc_pipes(px);
	pids = malloc(sizeof(pid_t) * px->cmd_count);
	if (!pids)
		error_exit("malloc");
	i = 0;
	while (i < px->cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			error_exit("fork");
		if (pids[i] == 0)
			run_child(px, i);
		i++;
	}
	close_fds(px);
	wait_and_exit(px, pids);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	px;

	if (argc >= 2 && ft_strncmp(argv[1], "here_doc", 8) == 0
		&& !argv[1][8])
	{
		if (argc < 6)
			usage_exit("Usage: ./pipex here_doc LIMITER cmd1 cmd2 file");
		px.cmd_start = 3;
		px.cmd_count = argc - 4;
		px.here_doc = 1;
	}
	else
	{
		if (argc < 5)
			usage_exit("Usage: ./pipex file1 cmd1 cmd2 file2");
		px.cmd_start = 2;
		px.cmd_count = argc - 3;
		px.here_doc = 0;
	}
	px.argv = argv;
	px.envp = envp;
	open_infile(&px, argv);
	open_outfile(&px, argv, argc);
	run_pipex(&px);
	return (0);
}
