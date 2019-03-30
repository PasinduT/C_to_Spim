		.data
prompt0:	.asciiz "Hello world\n"
prompt1:	.asciiz "Something else"
prompt2:	.asciiz "Another something else\n"
prompt3:	.asciiz "Also hello\n"

		.text
main:
		li $s0, 3

		move $t1, $s0
		li $t0, 4
		slt $t1, $t0, $t1
		beq $t1, $zero, ELSE0

		la $a0, prompt0
		li $v0, 4
		syscall

		j EXIT0
ELSE0:
		move $t1, $s0
		li $t0, 2
		bne $t0, $t1, ELSE1
		la $a0, prompt1
		li $v0, 4
		syscall

		j EXIT1
ELSE1:
		la $a0, prompt2
		li $v0, 4
		syscall

EXIT1:
EXIT0:
		la $a0, prompt3
		li $v0, 4
		syscall

		li $v0, 0
		j $ra

