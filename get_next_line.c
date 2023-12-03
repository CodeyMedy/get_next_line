/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 08:49:32 by mboukour          #+#    #+#             */
/*   Updated: 2023/12/03 02:05:49 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"


char *get_next_line(int fd)
{
    static char *save;
    static int reached_last;
    char *buffer;
    char *newline;
    char *line;
    int bytes_read;
    char *temp;

    if ((fd < 0 || fd > OPEN_MAX) || BUFFER_SIZE <= 0)
        return (NULL);
    buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
    if (!buffer)
        return (NULL);
    newline = ft_strchr(save, '\n');
    while(!newline)
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read == -1)
        {
            free(buffer);
            buffer = NULL;
            free(save);
            save = NULL;
            return (NULL);
        }
        if (bytes_read == 0)
        {
            free(buffer);
            buffer = NULL;
            if (!reached_last && save != NULL)
            {
                if (save[0] == '\0')
                {
                    free(save);
                    save = NULL;
                    return (NULL);
                }
                line = ft_strdup(save);
                if(!line)
                {
                    free(save);
                    save = NULL;
                    return (NULL);
                }
                reached_last = 1;
                free(save);
                save = NULL;
                return (line);
            }
            return (NULL);
        }
        buffer[bytes_read] = '\0';
        temp = save;
        save = ft_strjoin(save, buffer);
        if(!save)
            {
                free(temp);
                return (NULL);
            }
        free(temp);
        newline = ft_strchr(save, '\n');
    }
    free(buffer);
    buffer = NULL;
    if(newline)
    {
        line = ft_substr(save, 0, (newline - save) + 1);
        if(!line)
            {
                free(save);
                save = NULL;
                return (NULL);
            }
        temp = save;
        save = ft_strdup(newline + 1);
        if(!save)
            {
                free(temp);
                return (NULL);
            }
        free(temp);
    }
    else
    {
        line = ft_strdup(save);
        if(!line)
        {
            free(save);
            save = NULL;
            return (NULL);
        }
    }
    return (line);
}

