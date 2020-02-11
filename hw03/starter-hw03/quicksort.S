.global main
.data
bad_input: .string "bad input"
scan_fmt: .string "%ld"
print_fmt: .string "%ld\n"
print_num_fmt: .string "%ld "
new_line: .string "\n"
.text

//void swap(long* xs, long ii, long jj) //xs is %rdi, ii is %rsi, jj is %rdx
swap:
	/*
	    if (ii != jj) {
		long tmp = xs[ii];   //temp is %r10
		xs[ii] = xs[jj];
		xs[jj] = tmp;
	    }
	*/
	enter $0, $0
	cmp %rsi, %rdx
	je swap_done
	mov (%rdi, %rsi, 8), %r10
	mov (%rdi, %rdx, 8), %r11
	mov %r11, (%rdi, %rsi, 8)
	mov %r10, (%rdi, %rdx, 8)

swap_done:
	leave
	ret


//long partition(long* xs, long lo, long hi) //xs is %rdi, lo is %r12, hi is %r13
partition:
	push %r12
	push %r13
	enter $0, $0

	/*
	    long pivot = xs[hi - 1];  //pivot is %rbx

	    long ii = lo;  //ii is r8
	    long jj = lo;  //jj is r9
	    for (; jj < (hi - 1); ++jj) {
		if (xs[jj] < pivot) {
		    swap(xs, ii, jj);
		    ++ii;
		}
	    }
	    swap(xs, ii, hi - 1);
	    return ii;
	*/
	mov %rsi, %r12  //lo is %r12
	mov %rdx, %r13  //hi is %r13


	dec %rdx
	mov (%rdi, %rdx, 8), %rbx
	mov %rsi, %r8
	mov %rsi, %r9
	
partition_loop:
	mov %r13, %r10
	dec %r10
	cmp %r10, %r9
	jge partition_loop_end
	
	cmp %rbx, (%rdi, %r9, 8)
	jge partition_if_end
	
	mov %r8, %rsi
	mov %r9, %rdx
	call swap

	inc %r8
	

partition_if_end:
	inc %r9
	jmp partition_loop
	
partition_loop_end:

/*
    swap(xs, ii, hi - 1);
    return ii;
*/
	mov %r8, %rsi
	mov %r13, %r10
	dec %r10
	mov %r10, %rdx
	call swap
	mov %r8, %rax
	
	leave
	pop %r13
	pop %r12
	ret

//quicksort(long* xs, long lo, long hi)  %rdi, %r12, %r13
quicksort:
	/*
	    	if (hi - lo < 1) {
			return;
		}	
	*/
	push %rbx  //%rbx is pp
	push %r12  //lo
	push %r13  //hi
	enter $8, $0
	mov %rsi, %r12  //lo is %r12
	mov %rdx, %r13  //hi is %r13

	mov %r13, %r10
	sub %r12, %r10 //%r10 is now hi - lo
	cmp $1, %r10
	jl quicksort_end
	
	/*
	    long pp = partition(xs, lo, hi);
	    quicksort(xs, lo, pp);
	    quicksort(xs, pp + 1, hi);
	*/

	call partition
	mov %rax, %rbx  //pp
	
	mov %r12, %rsi
	mov %rbx, %rdx
	call quicksort

	mov %rbx, %r10 
	inc %r10  	//%r10 is now pp + 1
	mov %r10, %rsi
	mov %r13, %rdx
	call quicksort


quicksort_end:
	leave
	pop %r13
	pop %r12
	pop %rbx
	ret

//read_int(long* ptr)  
read_int:
	/*
	    	if (scanf("%ld", ptr) != 1) {
			puts("bad input");
			exit(1);
    		}
	*/
	enter $0, $0
	
	mov %rdi, %rsi  //ptr is %rsi
	mov $scan_fmt, %rdi
	mov $0, %al
	call scanf

	cmp $1, %rax
	jne read_bad_input
	
	leave
	ret

read_bad_input:
	mov $bad_input, %rdi
	mov $0, %al	
	call puts
	mov $1, %rdi
	call exit



//int main(int _argc, char* _argv[])
main:
	push %rbx
	push %r12
	push %r13
 	enter $8, $0

/*
	long nn; //nn is %rbx
    	read_int(&nn);
*/
	lea 8(%rsp), %rdi
	call read_int
	mov 8(%rsp), %rbx

//    long* xs = malloc(nn * sizeof(long));  xs is %r12
	mov 8(%rsp), %rax
	mov $8, %r10
	imul %r10
	mov %rax, %rdi
	call malloc
	mov %rax, %r12

/*
    for (long ii = 0; ii < nn; ++ii) {   // ii is %r13
        read_int(&(xs[ii]));
    }
*/
	mov $0, %r13
main_loop:
	cmp %rbx, %r13
	jge main_loop_done
	
	lea (%r12, %r13, 8), %rdi
	call read_int	

	inc %r13
	jmp main_loop

main_loop_done:
	
//     quicksort(xs, 0, nn);
	mov %r12, %rdi
	mov $0, %rsi
	mov %rbx, %rdx
	call quicksort

/*
    printf("%ld\n", nn);
*/
	mov $print_fmt, %rdi
	mov %rbx ,%rsi
	mov $0, %al
	call printf

/*
    for (long ii = 0; ii < nn; ++ii) {
        printf("%ld ", xs[ii]);
    }
*/
	mov $0, %r13
print_sorted_loop:
	cmp %rbx, %r13
	jge print_sorted_loop_end

	mov $print_num_fmt, %rdi
	mov (%r12, %r13, 8) ,%rsi
	mov $0, %al
	call printf

	inc %r13
	jmp print_sorted_loop

print_sorted_loop_end:

/*
    printf("\n");
    free(xs);
    return 0;
*/
	mov $new_line, %rdi
	mov $0, %al
	call printf
	
	mov %r12, %rdi
	call free
	mov $0, %rax

	leave
	pop %r12
	pop %r13
	pop %rbx


	ret


