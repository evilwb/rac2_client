#include <iostream>
#include <string>

#include "readerwriterqueue.h"
#include "pine/pine.h"
#include "APCpp/Archipelago.h"

#include "rac2_interface.h"

using namespace std;
using namespace PINE;
using namespace moodycamel;
using namespace Rac2;

int main(int, char **)
{
    const std::vector<Rac2_interface::Item> *inv = nullptr;
    try {
        Rac2_interface *rac2 = new Rac2_interface();
        ReaderWriterQueue<int> q(100);
        AP_Init("test");

        inv = rac2->list_inventory();
        cout << "Items in inventory: ";
        for (const auto& item : *inv) {
            cout << item.name << ", ";
        }
        cout << '\n';
    } catch (Shared::IPCStatus error) {
        cout << "PCSX2 connection error\n";
    }
    //delete inv;
}
