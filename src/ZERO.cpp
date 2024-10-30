#include "ZERO.h"
ZERO::ZERO() : Zero(0){

}
//to insure that a write happens before any read. we flip the old syncronization method
void ZERO::ZeroInput(uint32_t readdata1 , uint32_t readdata2 , bool Z) {
	Zero = (Z) ? (readdata1 == readdata2) : (readdata1 != readdata2);
	
}

bool ZERO::ZeroOutput(){
	
	return Zero;
}