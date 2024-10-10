#pragma once
#ifndef EXECUTESTAGE_H
#define EXECUTESTAGE_H
#include "GlobalClock.h"
#include "IDEXE.h"
#include <thread>
class GlobalClock;
class IDEXE;


class ExecuteStage {
private: 
	std::thread ExecuteThread;
	IDEXE* pipe; 
	GlobalClock* clk;
	void Executejob(); 

public:
	ExecuteStage(GlobalClock* clock, IFID* pipe);
	~ExecuteStage();
};









#endif