.text
main:
addi $t0,$0, 0x0
addi $t1,$0, 0x0
add $t2, $t0, $t1
addi $t0, $0,0x0
addi $t1,$0, 0x10
loop:
addi $t0, $t0, 0x1
addi $t4, $t0, 0x1
bne $t0, $t1, loop
