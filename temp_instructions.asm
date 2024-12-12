.text
main:
ADDI $1, $0, A
ORI $2, $0, 0xB
XORI $3, $0, 0xC
ADD $4, $1, $2
SUB $5, $4, $3
AND $6, $1, $3
OR $7, $2, $3
NOR $8, $2, $3
XOR $9, $1, $2
SLT $10, $1, $2
SGT $11, $3, $1
SLL $12, $1, 2
SRL $13, $2, 1
LW $14, 0x0($0)
SW $4, 0x0($0)
