.global main
.text



main:
	push %r12  //%r12 is argv
	push %r13  //%r13 is num1
	push %r14  //%r14 is num2
	enter $8, $0

	mov %rsi, %r12  //argv is %rsi -> %r12
	/*
	if (argc != 4) {
		puts("Usage:\n  ./ccalc N op N");
        	return 1;
    	}
	*/

	cmp $4, %rdi
	je main_good_args

usage:
	mov $usage_msg, %rdi
	call puts
	mov $1, %rax
	jmp main_done

main_good_args:
	//long num1 = atol(argv[1]);
	mov 8(%r12), %rdi
	call atol
	mov %rax, %r13  //%r13 is num1
	
	//long num2 = atol(argv[3]);
	mov 24(%r12), %rdi
	call atol
	mov %rax, %r14  //num2

	mov 16(%r12), %rax
	mov (%rax), %al

	cmp $'+', %al  //if (*argv[2] == '+')
	je add_num

	cmp $'-', %al  //else if (*argv[2] == '-') {
	je sub_num

	cmp $'*', %al  //else if (*argv[2] == '*') {
	je mul_num

	cmp $'/', %al  //else if (*argv[2] == '/') {
	je div_num


	/*
	else {
		puts("Usage:\n  ./ccalc N op N");
        	return 1;
	}
	*/
	jmp usage


add_num:
	mov %r14, %rax
	add %r13, %rax
	mov %rax, %rcx	

	mov $add_msg, %rdi
	jmp print_result
	 

sub_num:
	mov %r13, %rax
	sub %r14, %rax  //%rax -= %r14   ->  num1 = num1 - num2
	mov %rax, %rcx
	
	mov $sub_msg, %rdi
	jmp print_result

	

mul_num:
	mov %r14, %rax
	imul %r13, %rax
	mov %rax, %rcx	

	mov $mul_msg, %rdi
	jmp print_result

div_num:
	mov %r13, %rax
	idiv %r14
	mov %rax, %rcx

	mov $div_msg, %rdi
	jmp print_result


print_result:
	mov %r13, %rsi
	mov %r14, %rdx

	mov $0, %al
	call printf
	
	mov $0, %rax
	
main_done:
	leave
	pop %r14 	
	pop %r13
	pop %r12


	ret

.data
usage_msg:  .string "Usage:\n  ./ccalc N op N"

add_msg:  .string  "%ld + %ld = %ld\n"
sub_msg:  .string  "%ld - %ld = %ld\n"
mul_msg:  .string  "%ld * %ld = %ld\n"
div_msg:  .string  "%ld / %ld = %ld\n"
