	.section	__TEXT,__text,regular,pure_instructions
	.globl	_exponent
	.p2align	4, 0x90

	# This function takes three parameters:
	#   -- a 32-bit integer x in the %edi register
	#   -- a 32-bit integer y in the %esi register	
	#   -- a 32-bit integer n in the %edx register
	# It returns x^n + y^n, that is, x to the nth power plus y to the nth power.
	# The return value, being 32 bits, must be placed in the %eax register.
	
_exponent:	

	# Note: You can overwrite the 64-bit registers %rax, %rcx, %rdx, %rsi, %rdi, %r8,
	# %r9, %r10, %r11 (as well as their 32-bit halves, %eax, %ecx,
	# %edx, %esi, %edi, %r8d, %r9d, %r10d, %r11d) as you like. These are
	# "caller-saved" registers.
	
	pushq	%rbp	     # LEAVE THIS ALONE
	movq	%rsp, %rbp   # LEAVE THIS ALONE

	mov $1, %eax   # use the 32-bit register %eax to hold x^n, initially 1
	                     # 
	
    mov $1, %r10d   # use a 32-bit register to hold y^n, initially 1
	mov $0, %r11d   # use a 32-bit register to hold i, initially 0
LOOP_TOP:
        cmp %edx, %r11d   # compare i to n (remember, the comparison appears reversed)
        jge DONE   # and if i is greater or equal to n, jump to DONE

        imul %edi, %eax   # multiply register holding x^n by x
        imul %esi, %r10d   # multiply register holding y^n by y 
        inc %r11d   # i++
        jmp LOOP_TOP   # jump to LOOP_TOP
DONE:
        add %r10d, %eax   # add y^n to x^n
        		     # Since the result is already in %eax, just leave it there

	popq	%rbp	     # LEAVE THIS ALONE
	retq                 # LEAVE THIS ALONE

	
