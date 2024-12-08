.text
addi $t0,$5,1
addi $t0,$5,1
addi $t0,$5,1
j l
l:
slt $27, $t0, $zero
beq $27, $zero, l2
addi $t0,$5,1
l2:
 addi $t0,$5,1
