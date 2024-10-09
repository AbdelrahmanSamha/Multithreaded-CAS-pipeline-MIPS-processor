#include<iostream>
#include"GlobalClock.h"
#include"ConsoleLogger.h"
GlobalClock::GlobalClock(int num_threads)
	: tick(false), ClockCycles(0), waiting_threads_to_arrive(0), waiting_threads_to_leave(0), sync_barrier(num_threads) {
	//tick = false;
	//ClockCycles = 0;
	//waiting_threads = 0;
	//sync_barrier = std::barrier<>(num_threads);
}

void GlobalClock::clockTick() {

	while (waiting_threads_to_arrive.load() < 2) //needs to be changed once all the sages are set to 5, now working on Fetch&decode.
	{

		getClockCycles();
		std::this_thread::sleep_for(std::chrono::nanoseconds(100));
	}

	ClockCycles++;
	std::cout << "\n===========================================================================\n Clock Cycle count = " + std::to_string(ClockCycles) << std::endl;

	tick = true;


	while (waiting_threads_to_leave < 2) { //needs to be changed once all the sages are set to 5, now working on Fetch&decode.

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