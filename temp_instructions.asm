.text
addi $t0, $zero, 0x10
addi $t1, $zero, 0x5
or $t2, $t1, $t0
xor $t3, $t1, $t0
nor $t4, $t1, $t0
slt $t5, $t1, $t0
ori $s0, $zero, 0x10
xori $s1, $zero, 0x10
sll $s2, $t1, 0x4
srl $s3, $t1, 0x4
slti $s4, $t0, 0x20
sgt $s5, $t0, $t1
beq $t0, $t1, L
bne $t0, $t1, lab
slti $k0, $t0, 0x20
sgt $k1, $t0, $t1
L:
addi $t8, $s5, 0x1
lab:
addi $t8, $s4, 0x2
