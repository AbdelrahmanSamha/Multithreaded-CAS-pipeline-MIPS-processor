main:
addi $t0,$zero, 0
   addi $t1,$zero, 1
   addi $t2,$zero, 10
 addi $t3,$zero, 0
 addi $t4,$zero, 0
 
 store_loop: 
 sw $t1, 0($t3)
 addi $t1, $t1, 1
  addi $t3, $t3, 1
   addi $t0, $t0, 1
  bne $t0, $t2, store_loop
  
 addi $t0,$zero, 0
 addi $t3,$zero, 0
 sum_loop:
 lw $t5, 0($t3)
 add $t4, $t4, $t5
 addi $t3, $t3, 1
 addi $t0, $t0, 1
 bne $t0, $t2, sum_loop