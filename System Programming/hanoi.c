#include <stdio.h>
void hanoi(int n, char *a, char *b, char *c)
{
  if (n==1)                  /* step 1 */
  {
    printf("move disk 1 from %s to %s\n", a,c);
    return;
  }
  hanoi(n-1, a,c,b);
  printf("move disk %d from %s to %s\n", n, a,c);
  hanoi(n-1, b,a,c);
}
main()
{
  hanoi(3, "A", "B", "C");  /* 3 disks */
}

