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

program_5_2:
	move $fp, $sp
program_5_2_label0:
	move $t0, $a0
	li $t1, 1
	add $t2, $t0, $t1
	li $t1, 1
program_5_2_label3:
	beqz $t2, program_5_2_label
	li $t2, 1
	sub $t3, $t0, $t2
	move $t0, $t3
	li $t0, 1
	add $t2, $t1, $t0
	move $t1, $t2
	j program_5_2_label3
