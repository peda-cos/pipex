/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: peda-cos <peda-cos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 12:00:00 by peda-cos          #+#    #+#             */
/*   Updated: 2026/04/09 00:00:00 by peda-cos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static size_t	gnl_strlen(const char *str)
{
	size_t	length;

	length = 0;
	if (!str)
		return (0);
	while (str[length])
		length++;
	return (length);
}

static char	*gnl_strchr(const char *str, int target)
{
	int		index;
	char	target_char;

	index = 0;
	target_char = (char)target;
	if (!str)
		return (NULL);
	while (str[index])
	{
		if (str[index] == target_char)
			return ((char *)(str + index));
		index++;
	}
	if (target_char == '\0')
		return ((char *)(str + index));
	return (NULL);
}

static char	*gnl_strdup(const char *source)
{
	char	*duplicate;
	size_t	index;
	size_t	length;

	if (!source)
		return (NULL);
	length = gnl_strlen(source);
	duplicate = malloc(sizeof(char) * (length + 1));
	if (!duplicate)
		return (NULL);
	index = 0;
	while (source[index])
	{
		duplicate[index] = source[index];
		index++;
	}
	duplicate[index] = '\0';
	return (duplicate);
}

static char	*gnl_strjoin(char *prefix, char *suffix)
{
	char	*result;
	size_t	total_length;
	size_t	pos;
	size_t	idx;

	if (!prefix && !suffix)
		return (NULL);
	total_length = gnl_strlen(prefix) + gnl_strlen(suffix);
	result = malloc(sizeof(char) * (total_length + 1));
	if (!result)
		return (NULL);
	pos = 0;
	idx = 0;
	while (prefix && prefix[idx])
		result[pos++] = prefix[idx++];
	idx = 0;
	while (suffix && suffix[idx])
		result[pos++] = suffix[idx++];
	result[pos] = '\0';
	if (prefix)
		free(prefix);
	return (result);
}

static char	*gnl_substr(char const *source, unsigned int start, size_t max_len)
{
	char	*substring;
	size_t	index;
	size_t	source_length;

	if (!source)
		return (NULL);
	source_length = gnl_strlen(source);
	if (start >= source_length)
		return (gnl_strdup(""));
	if (max_len > source_length - start)
		max_len = source_length - start;
	substring = malloc(sizeof(char) * (max_len + 1));
	if (!substring)
		return (NULL);
	index = 0;
	while (index < max_len && source[start + index])
	{
		substring[index] = source[start + index];
		index++;
	}
	substring[index] = '\0';
	return (substring);
}

static int	find_newline_position(const char *str)
{
	int	pos;

	pos = 0;
	while (str[pos] && str[pos] != '\n')
		pos++;
	return (pos);
}

static char	*read_until_newline(int fd, char *stash)
{
	char	*read_buffer;
	char	*new_stash;
	ssize_t	bytes_read;

	read_buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!read_buffer)
		return (NULL);
	bytes_read = 1;
	while (!gnl_strchr(stash, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, read_buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(read_buffer);
			free(stash);
			return (NULL);
		}
		read_buffer[bytes_read] = '\0';
		new_stash = gnl_strjoin(stash, read_buffer);
		if (!new_stash)
			return (free(read_buffer), free(stash), NULL);
		stash = new_stash;
	}
	free(read_buffer);
	return (stash);
}

static char	*extract_line_from_stash(char *stash)
{
	char	*line;
	int		line_length;

	if (!stash || !stash[0])
		return (NULL);
	line_length = find_newline_position(stash);
	if (stash[line_length] == '\n')
		line_length++;
	line = gnl_substr(stash, 0, line_length);
	return (line);
}

static char	*get_remaining_after_line(char *stash)
{
	char	*remaining;
	int		line_length;

	line_length = find_newline_position(stash);
	if (!stash[line_length])
	{
		free(stash);
		return (NULL);
	}
	line_length++;
	remaining = gnl_strdup(stash + line_length);
	free(stash);
	return (remaining);
}

char	*get_next_line(int fd)
{
	static char	*stash[MAX_FD];
	char		*line;

	if (fd < 0 || fd >= MAX_FD || BUFFER_SIZE <= 0)
		return (NULL);
	stash[fd] = read_until_newline(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = extract_line_from_stash(stash[fd]);
	stash[fd] = get_remaining_after_line(stash[fd]);
	return (line);
}
