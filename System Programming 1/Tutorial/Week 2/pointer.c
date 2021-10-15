#include <stdio.h>

main ()
{
    int x = 100;  /* x is an integer variable */
    int *ptr = &x; /* ptr is a pointer to a integer x */
    printf ("x = %d\n", x);
    printf ("ptr = %p\n", ptr);
}

