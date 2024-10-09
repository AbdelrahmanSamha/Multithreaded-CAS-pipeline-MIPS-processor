#pragma once
#ifndef IFID_H
#define IFID_H
#include "FetchStage.h"
#include "DecodeStage.h" 
#include <thread>
#include <semaphore>
class FetchStage; // Forward declaration of GlobalClock
class DecodeStage;        // Forward declaration of IFID

class IFID {
private:
	FetchStage* Fetchthread;
	DecodeStage* Decodethread;
	std::binary_semaphore s1;  // Binary semaphore for write
	std::binary_semaphore s2;  // Binary semaphore for read
	uint32_t PC = 0x00000000;	//Programcounter
	uint32_t MC = 0x00000000; //MachineCode

public:
	IFID();
	void writedata(uint32_t PCin, uint32_t MCin);
	void readdata(uint32_t& PCout, uint32_t& MCout);
};

#endif 