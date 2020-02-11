.global main


.text
//long fib(long n)  n is %r12
/*
	if (n <= 1) 
	{
		return n;
	}
	return fib(n - 1) + fib(n - 2);
*/
fib:
	push %rdi  //n
	push %r13  //fib(n - 1)
	enter $0, $0

	mov %rdi, %rax
	cmp $1, %rdi
	jle fib_done
	


	dec %rdi
	call fib
	mov %rax, %r13

	dec %rdi
	call fib

	add %r13, %rax  // %rax = %r13 + %rax = fib(n - 1) + fib(n - 2)
	
	

fib_done:
	leave
	pop %rdi
	pop %r13	

	ret



//main(int argc, char* argv[])

/*
    if (argc != 2 || atol(argv[1]) < 0) {
        printf("Usage:\n  %s N, where N > 0\n", argv[0]);
        return 1;
*/

/*
    printf("fib(%ld) = %ld\n", atol(argv[1]), fib(atol(argv[1])));
    return 0;
*/



main:
	/*
	    if (argc != 2 || atol(argv[1]) < 0) {
		printf("Usage:\n  %s N, where N > 0\n", argv[0]);
		return 1;
	    }
	*/
	push %r12  //argv
	push %r14  //atol(argv[1])
	push %r15  //fib(atol(argv[1]))
	enter $8, $0
	mov %rsi, %r12

	cmp $2, %rdi
	jne usage

	mov 8(%r12), %rdi
	call atol
	cmp $0, %rax
	jl usage

	jmp main_fib

usage:
	mov $usage_msg, %rdi
	mov (%r12), %rsi
	mov $0, %al
	call printf
	mov $1, %rax
	jmp main_end

main_fib:
	/*
		printf("fib(%ld) = %ld", atol(argv[1]), fib(atol(argv[1])));
    		return 0;
	*/

	mov 8(%r12), %rdi
	call atol
	mov %rax, %r14
	
	mov %r14, %rdi
	call fib
	mov %rax, %r15

	
	mov $output_msg, %rdi
	mov %r14, %rsi
	mov %r15, %rdx
	mov $0, %al
	call printf
	jmp main_end

main_end:
	leave
	pop %r12
	pop %r14
	pop %r15
	ret







.data
usage_msg:  .string "Usage:\n %s N, where N > 0\n"

output_msg:  .string  "fib(%ld) = %ld\n"
