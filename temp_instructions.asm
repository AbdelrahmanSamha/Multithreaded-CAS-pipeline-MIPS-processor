main:
    addi $t0, $zero, 5
    addi $t1, $zero, 10
    jal subroutine
    add $t2, $t0, $t1

subroutine:
    sub $t3, $t1, $t0
    addi $t4, $t3, 2
    addi $t4, $t3, 2
    jr $ra
    addi $t4, $t3, 2
	addi $t4, $t3, 2