		.data
prompt0:	.asciiz "Hello world\n"
prompt1:	.asciiz "Also hello\n"

		.text
main:
		li $s0, 4

		move $t1, $s0
		li $t0, 2
		bne $t0, $t1, EXIT0
		la $a0, prompt0
		li $v0, 4
		syscall

EXIT0:
		la $a0, prompt1
		li $v0, 4
		syscall

		li $v0, 0
		j $ra

