/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 00:00:00 by peda-cos          #+#    #+#             */
/*   Updated: 2026/04/10 00:00:00 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/wait.h>
# include "libft.h"

typedef struct s_pipex
{
	int		fd_in;
	int		fd_out;
	int		outfile_err;
	int		**pipes;
	int		cmd_count;
	int		cmd_start;
	int		here_doc;
	char	**envp;
	char	**argv;
}	t_pipex;

void	error_exit(char *msg);
void	cmd_not_found(char *cmd);
void	usage_exit(char *msg);
void	free_split(char **split);
void	free_partial_pipes(t_pipex *px, int count);
void	free_pipes(t_pipex *px);
void	execute_cmd(char *cmd, char **envp);
void	alloc_pipes(t_pipex *px);
void	run_child(t_pipex *px, int i);
void	close_all_pipes(t_pipex *px);
void	close_fds(t_pipex *px);
void	handle_heredoc(char *limiter, int *pipe_fd);

#endif
