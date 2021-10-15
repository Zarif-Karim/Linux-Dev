#include <stdio.h>

int square(int num)
{
    return num*num;
}
void printSquare(int (*f)(int))
{
    for(int i = 0; i < 5; i++)
    {
        printf("the square of %d is %d\n", i,(*f)(i));
    }
}
int main()
{
    printSquare(square);
    return (0);
}
