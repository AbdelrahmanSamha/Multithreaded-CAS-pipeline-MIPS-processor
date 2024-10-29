#pragma once
#include "ZERO.h"

class Jump {
public: 
	//input
	bool Zero, Jr, B; 
	uint32_t Instruction;
	uint32_t PC4 ;
	//output
	bool Flush;
	uint8_t JmuxSel;
public: 
	Jump();
	void JumpSignalF(uint32_t Instruction);
	void JumpSignalD(bool Zero, bool Jr);
	void JumpUnit();

	uint32_t Jaddress();


};