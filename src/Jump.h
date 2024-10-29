#include <semaphore>
#include <cstdint>

class Jump {
public:
    // Input data
    uint32_t Instruction, PC4; //recieved from the fetch stage
    bool AndGate, Jr; //recieved from the decode stage.

    // Output data
    bool Flush;
    uint8_t JmuxSel;
    uint32_t Jaddress;

    // Constructor
    Jump();

    // Functions to set inputs and perform logic
    void JumpSignalF(uint32_t instruction, uint32_t pc4);
    void JumpSignalD(bool zero, bool jr);
    void JumpUnit(); // Core logic function

private:
    // Semaphores for synchronization
    std::binary_semaphore fetchSemaphore{ 0 };
    std::binary_semaphore decodeSemaphore{ 0 };
};
