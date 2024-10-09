#pragma once
#ifndef GLOBALCLOCK_H
#define GLOBALCLOCK_H
#include <atomic>
#include <barrier>
#include<thread>
class GlobalClock {
private:
	bool tick;
	int ClockCycles;
	std::barrier<> sync_barrier;
	std::atomic<int> waiting_threads_to_arrive;
	std::atomic<int>waiting_threads_to_leave;
	//FetchStage* FetchThread; //important to signal a termination 

public:
	GlobalClock(int num_threads);
	void clockTick();
	void waitforClockTick();
	int getClockCycles();
	void clockcycleincrement();

};
#endif