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

program_5_1:
	move $fp, $sp
	move $t0, $a0
program_5_1_label1:
	li $t1, 0
	bgt $t0, $t1, program_5_1_label3
program_5_1_label3:
	li $t1, 1
	sub $t2, $t0, $t1
	move $t0, $t2
	j program_5_1_label1
