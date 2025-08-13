.data
	_prompt: .asciiz "Enter an integer:"
	_ret: .asciiz "\n"
.globl main
.text
	jal main
	li $v0, 10
	syscall

read:
	li $v0, 4
	la $a0, _prompt
	syscall
	li $v0, 5
	syscall
	jr $ra

write:
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, _ret
	syscall
	move $v0, $0
	jr $ra

program_6_2__1:
	move $fp, $sp
	li $t0, 1
	move $t1, $t2
	move $t2, $t3
