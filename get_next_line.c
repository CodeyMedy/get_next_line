/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 08:49:32 by mboukour          #+#    #+#             */
/*   Updated: 2023/12/03 10:52:17 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"




char *read_and_append(int fd, char **save, int *reached_last) 
{
    char *buffer;
    char *temp;
    int bytes_read;

    buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
    if (!buffer)
        return NULL;

    while (!ft_strchr(*save, '\n')) {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read == -1) {
            free(buffer);
            free(*save);  // Free the save buffer as well
            *save = NULL;
            return NULL;
        }
        if (bytes_read == 0) {
            free(buffer);
            if (!*reached_last && *save && (*save)[0] == '\0') {
                free(*save);
                *save = NULL;
            }
            return *save;
        }
        buffer[bytes_read] = '\0';
        temp = *save;
        *save = ft_strjoin(*save, buffer);
        free(temp);
    }
    free(buffer);
    return *save;
}

char *process_line(char **save, int *reached_last) {
    char *line;
    char *newline;
    char *temp;

    newline = ft_strchr(*save, '\n');
    if (newline) {
        line = ft_substr(*save, 0, newline - *save + 1);
        temp = *save;
        *save = ft_strdup(newline + 1);
        if (!*save) { // Check if ft_strdup failed
            free(temp);
            return NULL;
        }
        free(temp);
    } else {
        if (**save == '\0') {
            free(*save);
            *save = NULL;
            *reached_last = 1;
            return NULL;
        }
        line = ft_strdup(*save);
        if (!line) { // Check if ft_strdup failed
            free(*save);
            *save = NULL;
            return NULL;
        }
        free(*save);
        *save = NULL;
        *reached_last = 1;
    }
    return line;
}


char *get_next_line(int fd) {
    static char *save;
    static int reached_last;
    char *line;

    if (fd < 0 || fd > OPEN_MAX || BUFFER_SIZE <= 0)
        return NULL;

    save = read_and_append(fd, &save, &reached_last);
    if (!save)
        return NULL;

    line = process_line(&save, &reached_last);
    return line;
}