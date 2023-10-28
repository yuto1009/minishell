#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char *p = getenv("PATH");
    printf("%s\n", p);
    return (0);
}