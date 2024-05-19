#include <unistd.h>
#include <iostream>
#include <memory>
#include <string>

#include "APCpp/Archipelago.h"
#include "pine/pine.h"
#include "rac2_interface.h"
#include "readerwriterqueue.h"

using namespace std;
using namespace PINE;
using namespace moodycamel;
using namespace Rac2;

int main(int, char **) {
    unique_ptr<Rac2_interface> rac2 = make_unique<Rac2_interface>();
    bool rac2_connected = false;
    bool ap_connected = false;

    while (true) {
        while (!rac2_connected) {
            cout << "Attempting to connect to PCSX2...\n";
            if (rac2->connect()) {
                if (rac2->id == "SCUS-97268") {
                    cout << "Successfully connected to " << rac2->platform
                         << "\n"
                         << "Game found = " << rac2->title << " (" << rac2->id
                         << ")\n";
                    rac2_connected = true;
                }
            } else {
                sleep(1);
            }
        }

        sleep(1);
    }
    // delete inv;
}
