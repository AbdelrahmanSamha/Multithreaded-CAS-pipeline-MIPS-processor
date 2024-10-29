#pragma once

#include <cstdint>
#include <semaphore>

class ZERO {
public: 
	//constructor 
	ZERO();
	void ZeroInput(uint32_t readdata1, uint32_t readdata2, bool Z);
	bool ZeroOutput();
private: 
	std::binary_semaphore Z1;
	bool Zero; 



};