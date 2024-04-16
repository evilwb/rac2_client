#include <iostream>
#include "pine.h"

int main(int, char**){
    PINE::PCSX2 *ipc = new PINE::PCSX2();
    
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
