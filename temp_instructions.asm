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
        sgt $t4, $t0, $t6