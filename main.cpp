#include <unistd.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "APCpp/Archipelago.h"
#include "pine/pine.h"
#include "rac2_interface.h"
#include "readerwriterqueue.h"
#include "util.h"

using namespace std;
using namespace PINE;
using namespace moodycamel;
using namespace Rac2;

int main(int, char **) {
    ReaderWriterQueue<string> input_queue(20);
    thread input_reader([&]() {
        while (true) {
            string input;
            getline(cin, input);
            input_queue.enqueue(input);
        }
    });
    input_reader.detach();

    unique_ptr<Rac2_interface> rac2 = make_unique<Rac2_interface>();

    bool rac2_connected = false;
    bool ap_connected = false;

    while (true) {
        // Connect to PCSX2
        while (!rac2_connected) {
            cout << "Attempting to connect to PCSX2...\n";
            if (rac2->connect()) {
                if (rac2->id == "SCUS-97268") {
                    cout << "Successfully connected to " << rac2->platform
                         << "\n"
                         << "Game found = " << rac2->title << " (" << rac2->id
                         << ")\n";
                    rac2_connected = true;

                    if (!ap_connected) {
                        cout << "There is no active multiworld server "
                                "connection. Connect using command: /connect "
                                "server:port slotname";
                        cout << "\n> " << flush;
                    }
                }
            } else {
                sleep(2);
            }
        }

        // Proccess Input
        string input;
        if (input_queue.try_dequeue(input)) {
            if (input.front() == '/') {
                cout << "client command\n> " << flush;
                // TODO: handle client commands
            } else if (input.front() == '!') {
                cout << "server command\n> " << flush;
                // TODO: handle server commands
            } else {
                cout << input << "\n> " << flush;
                // TODO: handle say
            }
        }

        sleep(1);
    }
    // delete inv;
}
