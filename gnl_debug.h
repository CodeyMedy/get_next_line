# ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
# define BUFFER_SIZE 1000
# endif

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <limits.h>
# include <stdio.h> // to remove

char    *get_next_line(int fd);
char    *read_and_store(int fd, char **buffer);
char    *extract_line(char **buffer);
char    *fill_buffer(int fd, char *buffer);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_strdup(char *s1, int a);
char    *ft_strchr(char *s, int c);
char	*ft_substr(const char *s, unsigned int start, size_t len, int a);
size_t	ft_strlen(const char *s);

    
#endif