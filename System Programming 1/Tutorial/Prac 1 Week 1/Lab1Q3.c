#include <stdio.h>

int main(){
	int x, y, z;
	
	printf("Please enter three integer numbers\n");
	scanf("%d%d%d",&x, &y, &z);
	
	int max = 0;
	if(max < x) max = x;
	if(max < y) max = y;
	if(max < z) max = z;
	
	printf("Largest number of the three: %d\n\n", max);
	return 0;
}