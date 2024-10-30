#pragma once
#ifndef IFID_H
#define IFID_H
#include <thread>
#include <semaphore>

#include "DecodeStage.h" 
#include "FetchStage.h"
#include "HazardDetection.h"
#include "ControlUnit.h"
#include "Jump.h"
class IFID {
private:
	
	std::binary_semaphore s1;  // Binary semaphore raw
	
	uint32_t PC = 0x00000000;	//Programcounter
	uint32_t MC = 0x00000000; //MachineCode
	HazardDetection* HDU;
	Jump* JU;
	


public:
	IFID(HazardDetection* HDU,Jump* JU);
	void writedata(uint32_t PCin, uint32_t MCin);
	void readdata(uint32_t& PCout, uint32_t& MCout);
};

#endif 