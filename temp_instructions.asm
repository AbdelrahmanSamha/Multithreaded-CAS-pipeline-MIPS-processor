addi $t0, $zero, 10
addi $t1, $zero, 5
or $t2, $t1, $t0
xor $t3, $t1, $t0
nor $t4, $t1, $t0
slt $t5, $t1, $t0
ori $s0, $zero, 10
xori $s1, $zero, 10
sll $s2, $t1, 4
srl $s3, $t1, 4
slti $s4, $t0, 20
sgt $s5, $t0, $t1
beq $t0, $t1, L
bne $t0, $t1, lab
slti $k0, $t0, 20
sgt $k1, $t0, $t1
L:
addi $t8, $s5, 1
lab:
addi $t8, $s4, 2
