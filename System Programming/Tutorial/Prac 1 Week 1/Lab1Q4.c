#include <stdio.h>
int max(int,int,int);
int main(){
	int x, y, z;
	
	printf("Please enter three integer numbers\n");
	scanf("%d%d%d",&x, &y, &z);
	
	printf("Largest number of the three: %d\n\n", max(x,y,z));
	return 0;
}

int max(int a, int b, int c) {
	int max;
	if(b < a) max = a;
	else max = b;
	if(max < c) max = c;
	
	return max;
}