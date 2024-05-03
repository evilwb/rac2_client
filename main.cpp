#include <iostream>

#include "readerwriterqueue.h"
#include "pine/pine.h"

using namespace PINE;
using namespace moodycamel;

int main(int, char**){
    PCSX2 *ipc = new PINE::PCSX2();
    ReaderWriterQueue<int> q(100);
    
    try {
        std::cout << "PCSX2 version: " << ipc->Version() << "\n";
    } 
    catch (PINE::Shared::IPCStatus error) {
        if (error == PINE::Shared::IPCStatus::NoConnection) {
            std::cout << "Failed to get PCSX2 version - Cannot connect to the IPC socket.\n";
        }
        else {
            throw error;
        }
    }
    
}
