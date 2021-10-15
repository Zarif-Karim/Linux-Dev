#include <stdio.h>
#include <stdlib.h>

long factorial(int n) {
	if (n == 1) return n;
	return n * factorial(n - 1);
}

int main(int argc, char* argv[]) {
	int n;

	//for command line
	n = atoi(argv[1]);
	
	//for user input
	//printf("Find factorial for: ");
	//scanf("%d", &n);
	printf("Factorial of %d is: %ld\n", n, factorial(n));



	return 0;
}