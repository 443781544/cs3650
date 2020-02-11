#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char* argv[])
{
	if (argc != 4) {
		puts("Usage:\n  ./ccalc N op N");
        	return 1;
    	}
	
	long num1 = atol(argv[1]);
	long num2 = atol(argv[3]);
	
	if (*argv[2] == '+') {
		printf("%ld + %ld = %ld\n", num1, num2, num1 + num2);
	}
	else if (*argv[2] == '-') {
		printf("%ld - %ld = %ld\n", num1, num2, num1 - num2);
	}
	else if (*argv[2] == '*') {
		printf("%ld * %ld = %ld\n", num1, num2, num1 * num2);
	}
	else if (*argv[2] == '/') {
		printf("%ld / %ld = %ld\n", num1, num2, num1 / num2);
	}
	else {
		puts("Usage:\n  ./ccalc N op N");
        	return 1;
	}
	return 0;
    
}

