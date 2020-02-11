	.global cube

cube:
	enter $0, $0
	mov %rdi, %rax
	imul %rdi
	imul %rdi
	leave
	ret
