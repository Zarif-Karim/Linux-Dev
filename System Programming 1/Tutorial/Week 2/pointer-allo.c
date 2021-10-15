#include <stdio.h>

main ( )
{

int x,y,*p,*q, temp;
x=10, y=25, p=&y, q=p;
printf("x=%d, y=%d, &x=%p, &y=%p, p=%p, q=%p, &p=%p, &q=%p\n",x,y,&x, &y, p,q,&p,&q);
++*p;
printf("after ++*P\n x=%d, y=%d, &x=%p, &y=%p, p=%p, q=%p\n",x,y,&x, &y, p,q);
temp=*p--;

printf("*p--=%d\n",temp);
printf("x=%d, y=%d, &x=%p, &y=%p, p=%p, q=%p\n",x,y,&x, &y, p,q);

(*q)++;

printf("after (*q)++\n x=%d, y=%d, &x=%p, &y=%p, p=%p, q=%p\n",x,y,&x, &y, p,q);

*q=15;

printf("after *q=15\n x=%d, y=%d, &x=%p, &y=%p, p=%p, q=%p\n",x,y,&x, &y, p,q);

temp=*(p+2);

printf("*(p+2)=%d\n",temp);

printf("after *(p+2)\n x=%d, y=%d, &x=%p, &y=%p, p=%p, q=%p\n",x,y,&x, &y, p,q);

/* **(p+2); */ /*invalid*/

}
