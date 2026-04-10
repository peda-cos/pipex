/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 00:00:00 by peda-cos          #+#    #+#             */
/*   Updated: 2026/04/10 00:00:00 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

static char	**get_env_paths(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (ft_split(envp[i] + 5, ':'));
		i++;
	}
	return (NULL);
}

static void	try_paths(char *cmd, char **args, char **envp)
{
	char	**paths;
	char	*tmp;
	char	*path;
	int		i;

	paths = get_env_paths(envp);
	if (!paths)
		return ;
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		if (!tmp)
		{
			i++;
			continue ;
		}
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (path)
			execve(path, args, envp);
		free(path);
		i++;
	}
	free_split(paths);
}

void	execute_cmd(char *cmd, char **envp)
{
	char	**args;
	char	*name;

	args = ft_split(cmd, ' ');
	if (!args || !args[0] || !args[0][0])
	{
		free_split(args);
		cmd_not_found(cmd);
	}
	if (ft_strchr(args[0], '/'))
	{
		execve(args[0], args, envp);
		perror(args[0]);
		free_split(args);
		if (errno == EACCES)
			exit(126);
		exit(127);
	}
	name = ft_strdup(args[0]);
	try_paths(args[0], args, envp);
	free_split(args);
	cmd_not_found(name);
}
