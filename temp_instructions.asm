add $t0,$t1,$t2
addi $t2,$t2,7  
j loop
loop:
slt $t0,$t0,$t2
sgt $t4,$t0,$t6
slti $t0,$t0,5
sll $t0,$t4,5
beq $t7,$t1,loop
jal fib
sw $t9,2($t4)
lw $t9,2($t4)
lw $t5,4($t8)
fib:
        jr   $ra 