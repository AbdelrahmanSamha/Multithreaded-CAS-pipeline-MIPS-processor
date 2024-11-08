#include<iostream>
#include <string>
#include"GlobalClock.h"
#include"ConsoleLogger.h"
GlobalClock::GlobalClock(int num_threads)
	: tick(false), ClockCycles(0), waiting_threads_to_arrive(0), waiting_threads_to_leave(0), sync_barrier(num_threads) {}//constructor, initializing all necessary values.

void GlobalClock::clockTick() {

	while (waiting_threads_to_arrive.load() < 5) //determine the number of stages.
	{
		std::this_thread::sleep_for(std::chrono::nanoseconds(100));
	}

	ClockCycles++;
	std::string separator(200, '=');
	ConsoleLog(0, separator);
	ConsoleLog(0, "Clock Cycle count = ", std::dec,ClockCycles);
	tick = true;

	while (waiting_threads_to_leave < 5) { //determine number of stages.

		std::this_thread::sleep_for(std::chrono::nanoseconds(100));
	}



	tick = false;
	waiting_threads_to_arrive.store(0);
	waiting_threads_to_leave.store(0);

}

void GlobalClock::waitforClockTick() {


	// Increment the counter for waiting threads
	waiting_threads_to_arrive.fetch_add(1);

	// Wait at the barrier until all threads reach this point
	sync_barrier.arrive_and_wait();

	//make all threads wait until a new cycle starts
	while (tick == false) {

		std::this_thread::sleep_for(std::chrono::nanoseconds(100));

	}

	waiting_threads_to_leave.fetch_add(1);


	// Now you can proceed with the work
}

int GlobalClock::getClockCycles() {

	return ClockCycles;
}
void GlobalClock::clockcycleincrement() {
	ClockCycles++;
}