
.data
.text
add $t0, $t1, $t2
        sub $t0, $t1, $t2
        and $t0, $t1, $t2
        addi $t0, $t0, 2
                addi $t2, $t2, 7
           xori $t0, $t1, 7   
        j loop
        or $t0, $t1, $t2
        loop:
        j l3
        nor $t0, $t1, $t2
        j loop
        l3:
        xor $t0, $t1, $t2
        slt $t0, $t0, $t2
        slti $t0, $t0, 5
	sll $t0, $t4, 5
	andi $t0, $t1, 10
	beq $t7, $t1, loop
	jr $ra
sw $t9, 2($t4)
lw $t9, 2($t4)