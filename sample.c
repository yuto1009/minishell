#include <stdio.h>
#include <stdlib.h>
#include "libft/libft.h"

// char *trim_single_path(char *paths)
// {
//     size_t i;
//     char *path;

//     i = 0;
//     while(paths[i] != '\0')
//     {
//         if (paths[i] == ':')
//             break;
//         i++;
//     }
//     path = (char *)malloc(sizeof(char)*(i) + 1);
//     if (path == NULL)
//         return (NULL);
//     ft_strlcpy(path, paths, i+1);
//     return path;
// }

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q a.out");
// }
// int main(void)
// {
//     extern char **environ;
//     char *paths = getenv("PATH");
//     while(*paths)
//     {
//         char *path = trim_single_path(paths);
//         paths = ft_strchr(paths, ':');
//         if (paths == NULL)
//         {
//             printf("%s\n", path);
//             free(path);
//             break;
//         }
//         paths++;
//         printf("%s\n", path);
//         free(path);
//     }
//     return (0);
// }

int main(void)
{
    // Allocate memory for the concatenated string
    char p[9] = "Hello"; // Ensure this is large enough for both strings plus null terminator
    char *p2 = "DAY";

    // Use the function (assuming it behaves like strlcat)
    ft_strlcat(p, p2, 9);
    printf("%s\n", p);
    return (0);
}