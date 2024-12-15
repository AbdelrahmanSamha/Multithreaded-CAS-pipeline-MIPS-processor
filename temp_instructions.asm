.text
main:
addi $t0,$zero, 0x0
addi $t1,$zero, 0x1
addi $t2,$zero, 0x10
addi $t3,$zero, 0x0
addi $t4,$zero, 0x0
addi $t6,$zero, 0x0
store_loop:
sw $t1, 0x0($t3)
addi $t1, $t1, 0x1
addi $t3, $t3, 0x1
addi $t0, $t0, 0x1
bne $t0, $t2, store_loop
addi $t0,$zero, 0x0
addi $t3,$zero, 0x0
sum_loop:
lw $t5, 0x0($t3)
add $t4, $t4, $t5
lw $t7, 0x1($t3)
add $t6, $t6, $t7
addi $t3, $t3, 0x1
addi $t0, $t0, 0x2
bne $t0, $t2, sum_loop
add $s0,$t4, $t6
