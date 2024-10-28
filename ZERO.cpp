#include "ZERO.h"
ZERO::ZERO() : Z1(0){

}
//to insure that a write happens before any read. we flip the old syncronization method
void ZERO::ZeroInput(uint32_t readdata1 , uint32_t readdata2 , bool Z) {
	Zero = (Z) ? (readdata1 == readdata2) : (readdata1 != readdata2);
	Z1.release();
}

bool ZERO::ZeroOutput(){
	Z1.acquire();
	return Zero;
}