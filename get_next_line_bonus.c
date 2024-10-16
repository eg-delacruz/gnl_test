/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erde-la- <erde-la-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 18:13:26 by erde-la-          #+#    #+#             */
/*   Updated: 2024/10/16 13:20:30 by erde-la-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*ft_raw_str(int fd, char *buffer, char *rem_str, ssize_t b_read)
{
	char	*tmp;

	if (ft_strchr(rem_str, '\n'))
		return (rem_str);
	while (b_read > 0)
	{
		b_read = read(fd, buffer, BUFFER_SIZE);
		if (b_read == -1)
			return (ft_free((void **)&rem_str), NULL);
		else if (b_read == 0)
			break ;
		buffer[b_read] = '\0';
		tmp = rem_str;
		rem_str = ft_strjoin(tmp, buffer);
		if (!rem_str)
			return (ft_free((void **)&tmp), NULL);
		ft_free((void **)&tmp);
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (rem_str);
}

static char	*ft_clean_str(char *raw_line)
{
	char	*clean_str;
	int		i;

	i = 0;
	while (raw_line[i] && raw_line[i] != '\n')
		i++;
	if (raw_line[i] == '\0' && raw_line[i] != '\n')
		return (raw_line);
	clean_str = malloc(sizeof(char) * (i + 2));
	if (!clean_str)
		return (ft_free((void **)&raw_line), NULL);
	i = 0;
	while (raw_line[i] != '\n')
	{
		clean_str[i] = raw_line[i];
		i++;
	}
	clean_str[i] = '\n';
	clean_str[++i] = '\0';
	return (ft_free((void **)&raw_line), clean_str);
}

static char	*ft_remain_str(char *raw_line, char *old_rem_str)
{
	char	*rem_str;
	int		i;
	size_t	str_len;

	i = 0;
	while (raw_line[i] != '\n' && raw_line[i])
		i++;
	if (raw_line[i] == '\0')
	{
		rem_str = malloc(1);
		if (!rem_str)
			return (ft_free((void **)&raw_line), NULL);
		rem_str[0] = '\0';
		return (rem_str);
	}
	i++;
	str_len = ft_strlen(raw_line + i);
	rem_str = malloc(str_len + 1);
	if (!rem_str)
	{
		ft_free((void **)&old_rem_str);
		return (ft_free((void **)&raw_line), NULL);
	}
	ft_strlcpy(rem_str, raw_line + i, str_len + 1);
	return (rem_str);
}

static char	*ft_init_check(char *rem_str, int fd)
{
	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (ft_free((void **)&rem_str), NULL);
	if (rem_str == NULL)
	{
		rem_str = malloc(1);
		if (!rem_str)
			return (NULL);
		rem_str[0] = '\0';
	}
	return (rem_str);
}

char	*get_next_line(int fd)
{
	char		*line;
	char		*buffer;
	static char	*rem_str[1024];
	ssize_t		b_read;

	rem_str[fd] = ft_init_check(rem_str[fd], fd);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (ft_free((void **)&rem_str[fd]), NULL);
	if (rem_str[fd] == NULL)
		return (ft_free((void **)&buffer), NULL);
	b_read = 1;
	line = ft_raw_str(fd, buffer, rem_str[fd], b_read);
	ft_free((void **)&buffer);
	if (*line == '\0')
		return (ft_free((void **)&rem_str[fd]), NULL);
	rem_str[fd] = ft_remain_str(line, rem_str[fd]);
	if (!rem_str[fd])
		return (NULL);
	line = ft_clean_str(line);
	if (!line)
		return (ft_free((void **)&rem_str[fd]), NULL);
	return (line);
}
