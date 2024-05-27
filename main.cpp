#include "main.h"

#include <chrono>
#include <cstdint>
#include <ostream>
#include <thread>

#include "Archipelago.h"
#include "readerwriterqueue.h"

using namespace std;
using namespace PINE;
using namespace moodycamel;
using namespace Rac2;

int main(int, char **) {
    input_queue = ReaderWriterQueue<string>(20);
    input_reader = thread([&]() {
        while (true) {
            string input;
            getline(cin, input);
            input_queue.enqueue(input);
        }
    });
    input_reader.detach();

    rac2 = make_unique<Rac2_interface>();
    rac2_connected = false;

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

                    if (!(AP_IsInit() && AP_GetConnectionStatus() ==
                                             AP_ConnectionStatus::Connected)) {
                        printp(
                            "There is no active multiworld server connection. "
                            "Connect using command: /connect server:port "
                            "slotname");
                    }
                }
            } else {
                sleep(2);
            }
        }

        // Proccess Input
        string input;
        if (input_queue.try_dequeue(input)) {
            vector<string> tokens = tokenize(input);

            if (tokens[0] == "/help") {
                // TODO: print command help
                printp("Command help coming soon `\\_( ͡° ͜ʖ ͡°)_/¯");
            } else if (tokens[0] == "/connect" && tokens.size() == 3) {
                ap_connect(tokens[1], tokens[2]);
            } else if (tokens[0] == "connect" && tokens.size() == 4) {
                ap_connect(tokens[1], tokens[2], tokens[3]);
            } else {
                printp(
                    "Invalid command. Try using '/help' for a list of "
                    "valid commands");
            }
        }

        sleep(1);
    }
}

bool ap_connect(const std::string address, const std::string slotname,
                const string password) {
    if (AP_IsInit() &&
        AP_GetConnectionStatus() == AP_ConnectionStatus::Connected) {
        printp("Already connected to AP server");
    }

    AP_Init(address.c_str(), "RAC2", slotname.c_str(), password.c_str());

    AP_SetItemClearCallback([]() {
        // TODO: Clear all item states
    });

    AP_SetItemRecvCallback([](int64_t item_id, bool notify) {
        // TODO: add the item with the given ID to the current state. The
        // second parameter decides whether or not to notify the player
    });

    AP_SetLocationCheckedCallback([](int64_t location_id) {
        // TODO: mark the location with the given id as checked.
    });

    AP_Start();

    for (int i = 0; i < 5; i++) {
        if (AP_GetConnectionStatus() == AP_ConnectionStatus::Connected) {
            return true;
        }
        this_thread::sleep_for(chrono::seconds(1));
    }

    printp("Connection to AP server failed. Error: %d", int(AP_GetConnectionStatus()));
    AP_Shutdown();
    return false;
}
