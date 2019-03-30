#include <stdio.h>

int main()
{
    int a;
    a = 3;
    if (a > 4)
    {
        printf("Hello world\n");
    }
    else if (a == 2)
    {
        printf("Something else");
    }
    else
    {
        printf("Another something else\n");
    }
    printf("Also hello\n");
    return 0;
}