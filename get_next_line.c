#include "get_next_line.h"

/*
    strlen() function is used to find the length of a string.
    strchr() function is used to find the first occurrence of a character in a given string.
    strjoin() function is used to concatenate two strings.
    save_line() function is used to save the line in a static variable.
    print_line() function is used to print the line.
    read_line() function is used to read the line from the file.
*/

static char	*ft_saveline(char *tmp, int start)
{
	char	*restart;
	int		len;

	len = ft_strlen(tmp) - start;
	if (len <= 0)
	{
		free(tmp);
		return (NULL);
	}
	restart = ft_substr(tmp, start, len);
	free(tmp);
	return (restart);
}

static char	*ft_printline(char *tmp, int *start_next)
{
	char	*line;
	int		i;

	i = 0;
	while (tmp[i] && tmp[i] != '\n')
		i++;
	if (tmp[i] == '\n')
		*start_next = i + 1;
	else
	{
		*start_next = i;
		if (i == 0 && tmp[i] == '\0')
			return (NULL);
	}
	line = ft_substr(tmp, 0, *start_next);
	return (line);
}

static char	*ft_readline(int fd, char *buf, char *tmp)
{
	int		read_bytes;
	char	*new_tmp;

	while (!ft_strchr(tmp, '\n'))
	{
		read_bytes = read(fd, buf, BUFFER_SIZE);
		if (read_bytes == 0 && tmp[0] == '\0')
		{
			free(tmp);
			return (NULL);
		}
		if (read_bytes <= 0)
			return (tmp);
		buf[read_bytes] = '\0';
		new_tmp = ft_strjoin(tmp, buf);
		free(tmp);
		tmp = new_tmp;
	}
	return (tmp);
}

char	*get_next_line(int fd)
{
	char		*buf;
	char		*line;
	static char	*temp;
	int			start_next;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, 0, 0) < 0)
		return (NULL);
	if (!temp)
		temp = ft_strdup("");
	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	temp = ft_readline(fd, buf, temp);
	free(buf);
	if (!temp)
		return (NULL);
	line = ft_printline(temp, &start_next);
	if (!line && !temp)
		return (NULL);
	temp = ft_saveline(temp, start_next);
	return (line);
}

int	main(void)
{
	char	*line;
	int		fd;

	int i = 0;
	fd = open("txt.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}