00400000 20080000  addi $t0,$zero, 0
00400004 20090001     addi $t1,$zero, 1
00400008 200A000A     addi $t2,$zero, 10
0040000C 200B0000   addi $t3,$zero, 0
00400010 200C0000   addi $t4,$zero, 0
00400014 AD690000   sw $t1, 0($t3)
00400018 21290001   addi $t1, $t1, 1
0040001C 216B0001    addi $t3, $t3, 1
00400020 21080001     addi $t0, $t0, 1
00400024 150AFFFB    bne $t0, $t2, store_loop
00400028 20080000   addi $t0,$zero, 0
0040002C 200B0000   addi $t3,$zero, 0
00400030 8D6D0000   lw $t5, 0($t3)
00400034 018D6020   add $t4, $t4, $t5
00400038 216B0001   addi $t3, $t3, 1
0040003C 21080001   addi $t0, $t0, 1
00400040 150AFFFB   bne $t0, $t2, sum_loop
