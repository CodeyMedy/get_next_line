/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 08:49:32 by mboukour          #+#    #+#             */
/*   Updated: 2023/12/01 15:53:31 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


char *get_next_line(int fd)
{
    static char *save = NULL;
    static int reached_last = 0;
    char *buffer;
    char *newline;
    char *line;
    int bytes_read;

    if ((fd < 0 || fd > OPEN_MAX) || BUFFER_SIZE <= 0)
        return (NULL);
    buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
    if (!buffer)
        return (NULL);
    buffer[BUFFER_SIZE] = '\0';
    newline = ft_strchr(save, '\n');
    while(!newline)
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read == -1)
        {
            free(buffer);
            return (NULL);
        }
        if (bytes_read == 0)
        {
            free(buffer);
            if (!reached_last && save != NULL)
            {
                line = ft_strdup(save);
                reached_last = 1;
                return (line);
            }
            return (NULL);
        }
        buffer[bytes_read] = '\0';
        char *temp = save;
        save = ft_strjoin(save, buffer);
        free(temp);
        newline = ft_strchr(save, '\n');
    }
    free(buffer);

    if(newline)
    {
        line = ft_substr(save, 0, (newline - save) + 1);
        char *temp = save;
        save = ft_strdup(newline + 1);
        free(temp);
        if (!save)
            return (NULL);
    }
    else
    {
        line = ft_strdup(save);
        if(!line)
            return (NULL);
    }
    return (line);
}
