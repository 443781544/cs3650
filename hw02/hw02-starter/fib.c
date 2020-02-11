
#include <stdio.h>
#include <stdlib.h>

long fib(long n)
{
	if (n <= 1) 
	{
		return n;
	}
	return fib(n - 1) + fib(n - 2);
}

int
main(int argc, char* argv[])
{
    if (argc != 2 || atol(argv[1]) < 0) {
        printf("Usage:\n  %s N, where N > 0\n", argv[0]);
        return 1;
    }

    printf("fib(%ld) = %ld\n", atol(argv[1]), fib(atol(argv[1])));
    return 0;
}


