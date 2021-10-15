#include <stdio.h>
void swapit(int, int);
main()
{
  int a=1, b=2;
  printf("main: a is %d and b is %d\n", a, b);
  swapit(a, b);
  printf("main: after change, a is %d and b is %d\n", a, b);  
}

void swapit(int a, int b)
{ 
  int temp;
  temp=a;
  a=b;
  b=temp;
  printf("swapit: a is %d and b is %d\n", a,b);
}

