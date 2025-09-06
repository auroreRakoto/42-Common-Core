/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aurakoto <aurakoto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 18:53:51 by aurakoto          #+#    #+#             */
/*   Updated: 2023/05/15 10:25:19 by aurakoto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_fill_res(char *buffer, char *line, int len, int i)
{
	char	*res;

	res = (char *)malloc(sizeof(char) * len + i + 1);
	if (!res)
		return (NULL);
	res[len + i] = 0;
	while (i > 0)
	{
		res[i + len - 1] = buffer[i - 1];
		i--;
	}
	while (len > 0 && line)
	{
		res[len - 1] = line[len - 1];
		len--;
	}
	return (res);
}

char	*ft_get_line(char *line, char *buffer, int *pos, int *end_line)
{
	char	*res;
	int		i;
	int		len;
	int		new_pos;

	i = 0;
	len = 0;
	while (line && line[len])
		len++;
	while (buffer[*pos + i] && buffer[*pos + i] != '\n' && *pos < BUFFER_SIZE)
		i++;
	if (buffer[*pos + i] == '\n')
	{
		i++;
		*end_line = 1;
	}
	new_pos = *pos + i;
	res = ft_fill_res(buffer + *pos, line, len, i);
	if (*pos == BUFFER_SIZE)
		*pos = 0;
	else
		*pos = new_pos;
	free(line);
	return (res);
}

char	*ft_read_or_fill(int fd, char *buffer, int *pos, int *num_read)
{
	char	*line;
	int		end_line;

	line = NULL;
	end_line = 0;
	while (*num_read > 0 && !end_line)
	{
		if (buffer[*pos] == 0)
		{
			*num_read = read(fd, buffer, BUFFER_SIZE);
			buffer[*num_read] = 0;
			if (*num_read < 0)
			{
				free(line);
				return (NULL);
			}
			if (*num_read == 0)
				return (line);
		}
		line = ft_get_line(line, buffer, pos, &end_line);
		if (!line)
			return (NULL);
	}
	return (line);
}

void	ft_bzero(void *s, int n)
{
	char	*p;

	p = (char *)s;
	while (n--)
		*p++ = 0;
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	static int	pos;
	char		*line;
	int			num_read;

	num_read = 1;
	if (fd < 0 || read(fd, 0, 0) < 0)
	{
		buffer[0] = 0;
		pos = 0;
		return (NULL);
	}
	if (fd == 0 && buffer[pos] == '\0')
	{
		pos = 0;
		ft_bzero(buffer, BUFFER_SIZE);
	}
	line = ft_read_or_fill(fd, buffer, &pos, &num_read);
	return (line);
}
