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

program_5_4__3:
	move $fp, $sp
program_5_4__3_label0:
	li $t0, 1
	li $t1, 0
	beq $t0, $t1, program_5_4__3_label0
	j program_5_4__3_label
	j program_5_4__3_label0
