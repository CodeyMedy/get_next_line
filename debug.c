#include "gnl_debug.h"
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

// Global counter for active allocations
static int allocation_count = 0;

void *debug_malloc(size_t size, const char *file, int line)
{
    void *ptr = malloc(size);
    if (ptr != NULL) {
        allocation_count++;

        // Print allocation info
        printf("Malloc: %p of size %zu at %s:%d, %d allocations active\n", 
               ptr, size, file, line, allocation_count);

        // Getting the call stack
        void *callstack[128];
        int frames = backtrace(callstack, 128);
        char **strs = backtrace_symbols(callstack, frames);

        printf("Call stack:\n");
        for (int i = 0; i < frames; ++i) {
            printf("  %s\n", strs[i]);
        }

        free(strs); // Free the string pointers
    }
    return ptr;
}

// Macro to automatically fill in file and line information
#define DEBUG_MALLOC(size) debug_malloc(size, __FILE__, __LINE__)

void debug_free(void *ptr, const char *file, int line)
{
    if (ptr != NULL) {
        allocation_count--;
        printf("Free: %p at %s:%d, %d allocations active\n", 
               ptr, file, line, allocation_count);
    }
    free(ptr);
}

// Function for line allocation without affecting the allocation count
void *line_malloc(size_t size)
{
    return malloc(size); // Simply perform the allocation without modifying the count
}
// Macro to automatically fill in file and line information
#define DEBUG_FREE(ptr) debug_free(ptr, __FILE__, __LINE__)


size_t	ft_strlen(const char *s)
{
    int i;
	size_t	length;

    i = 0;
	length = 0;
	while (s[i])
	{
		length++;
		i++;
	}
	return (length);
}


char	*ft_strchr(char *s, int c)
{
	char	x;
	size_t	i;

    if(!s)
        return (NULL);
	x = (char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == x)
			return (&s[i]);
		i++;
	}
	if (x == '\0')
		return (&s[i]);
	return (NULL);
}

char	*ft_strdup(char *s1, int a)
{
	char	*copy;
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(s1);
    if (a == 1)
	    copy = (char *)DEBUG_MALLOC(sizeof(char) * (len + 1));
    else
        copy = (char *)line_malloc(sizeof(char) * (len + 1));
	if (!copy)
		return (NULL);
	while (i < len)
	{
		copy[i] = s1[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

char	*ft_substr(const char *s, unsigned int start, size_t len, int a)
{
	size_t	i;
	char	*substr;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup("", 1));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
    if (a == 1)
        substr = (char *)DEBUG_MALLOC((len + 1) * sizeof(char));
    else
        substr = (char *)line_malloc((len + 1) * sizeof(char));
	if (!substr)
		return (NULL);
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
    substr[i] = '\0';
	return (substr);
}


char	*ft_strjoin(char *s1, char *s2)
{
	char	*finalstr;
	int		i;
	int		j;

    if(!s1 && !s2)
        return (NULL);
    else if (!s1)
        return (ft_strdup(s2, 1));
    else if (!s2)
        return (ft_strdup(s1, 1));
	i = 0;
	j = 0;
	finalstr = (char *)DEBUG_MALLOC((ft_strlen(s1) + ft_strlen(s2) + 1)
			* sizeof(char));
	if (!finalstr)
    {
        return (NULL);
    }
	while (s1[i])
		finalstr[j++] = s1[i++];
	i = 0;
	while (s2[i])
	{
		finalstr[j] = s2[i];
		j++;
		i++;
	}
	finalstr[j] = '\0';
	return (finalstr);
}




char *get_next_line(int fd)
{
    static char *save;
    static int reached_last = 0;
    char *buffer;
    char *newline;
    char *line;
    int bytes_read;
    char *temp;

    if ((fd < 0 || fd > OPEN_MAX) || BUFFER_SIZE <= 0)
        return (NULL);

    buffer = (char *)DEBUG_MALLOC(sizeof(char) * (BUFFER_SIZE + 1));
    if (!buffer)
        return (NULL);

    newline = ft_strchr(save, '\n');
    while(!newline)
    {
        bytes_read = read(fd, buffer, BUFFER_SIZE);
        if (bytes_read == -1)
        {
            DEBUG_FREE(buffer);
            DEBUG_FREE(save);
            save = NULL;
            return (NULL);
        }

        if (bytes_read == 0)
        {
            DEBUG_FREE(buffer);
            if (!reached_last && save != NULL && save[0] != '\0')
            {
                line = ft_strdup(save, 0);
                if (!line)
                {
                    DEBUG_FREE(save);
                    save = NULL;
                    return (NULL);
                }
                reached_last = 1;
                DEBUG_FREE(save);
                save = NULL;
                return (line);
            }
            return (NULL);
        }
        buffer[bytes_read] = '\0';
        if (bytes_read == 1 && buffer[0] == '\n') {
            line = ft_strdup("\n", 0); // or however you create a new string in your environment
            DEBUG_FREE(save);       // Free previous save
            save = NULL;            // Reset save to NULL or an empty string
            DEBUG_FREE(buffer);
            return line;
        }
        temp = save;
        save = ft_strjoin(save, buffer);
        if (!save)
        {
            DEBUG_FREE(temp);
            DEBUG_FREE(buffer);
            return (NULL);
        }
        DEBUG_FREE(temp);
        newline = ft_strchr(save, '\n');
    }
    DEBUG_FREE(buffer);
    if (newline)
    {
        line = ft_substr(save, 0, (newline - save) + 1, 0);
        if (!line)
        {
            DEBUG_FREE(save);
            save = NULL;
            return (NULL);
        }
        temp = save;
        save = ft_strdup(newline + 1, 1);
        if (!save)
        {
            DEBUG_FREE(temp);
            return (NULL);
        }
        DEBUG_FREE(temp);
    }
    else
    {
        line = ft_strdup(save, 0);
        if (!line)
        {
            DEBUG_FREE(save);
            save = NULL;
            return (NULL);
        }
    }
    return (line);
}
