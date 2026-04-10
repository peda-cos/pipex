/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 00:00:00 by peda-cos          #+#    #+#             */
/*   Updated: 2026/04/10 00:00:00 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

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
	int		pipe_fd[2];
	char	**envp;
	char	**argv;
}	t_pipex;

void	error_exit(char *msg);
void	cmd_not_found(char *cmd);
void	free_split(char **split);
void	execute_cmd(char *cmd, char **envp);
void	child_one(t_pipex *px);
void	child_two(t_pipex *px);

#endif
