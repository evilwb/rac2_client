#include "rac2_interface.h"

#include <memory>

#include "address.h"
#include "pine.h"

using PINE::PCSX2;
using Rac2::Rac2_interface;

Rac2_interface::Rac2_interface() {}

bool Rac2_interface::connect() {
    ipc = std::make_shared<PCSX2>();

    try {
        // For some reason the Version call only works after PCSX2 starts
        // running a game.
        platform = ipc->Version();

        // These calls also only work when PCSX2 is running a game but it makes
        // more sense for these calls.
        title = ipc->GetGameTitle();
        id = ipc->GetGameID();
        version = ipc->GetGameVersion();
        uuid = ipc->GetGameUUID();

        return true;
    } catch (PCSX2::IPCStatus error) {
        return false;
    }
}

int Rac2_interface::give_item(Item item) {
    ipc->Write<uint8_t>(item.address, 1);
    return 0;
}

const std::vector<Rac2_interface::Item> *Rac2_interface::list_inventory() {
    std::vector<Item> *items = new std::vector<Item>;

    for (const auto &[_, item] : item_table) {
        try {
            uint8_t byte = ipc->Read<uint8_t>(item.address);
            if (byte == 1) {
                items->push_back(item);
            }
        } catch (PINE::Shared::IPCStatus error) {
            throw error;
            // TODO: deal with this
        }
    }

    return items;
}

// int get_pause_state() {
//     std::unique_ptr<PCSX2> ipc = std::make_unique<PCSX2>();

//     uint8_t pause_state = ipc->Read<uint8_t>(PAUSE_STATE);
//     printf("%02d\n", pause_state);

//     return pause_state;
// }

// int get_current_planet() {
//     std::unique_ptr<PCSX2> ipc = std::make_unique<PCSX2>();

//     uint8_t planet_number = ipc->Read<uint8_t>(CURRENT_PLANET);
//     printf("%02d - %s\n", planet_number,
//     PLANET_NAMES[planet_number].c_str());

//     return planet_number;
// }

// int get_ratchet_position() {
//     std::unique_ptr<PCSX2> ipc = std::make_unique<PCSX2>();

//     uint32_t ratchet_x = ipc->Read<uint32_t>(0x189EA0);
//     uint32_t ratchet_y = ipc->Read<uint32_t>(0x189EA4);
//     uint32_t ratchet_z = ipc->Read<uint32_t>(0x189EA8);

//     printf("%f, %f, %f\n", *(float*)&ratchet_x, *(float*)&ratchet_y,
//     *(float*)&ratchet_z); return 0;
// }

// int set_ratchet_position(float x, float y, float z) {
//     std::unique_ptr<PCSX2> ipc = std::make_unique<PCSX2>();
//     uint32_t x_bits, y_bits, z_bits;

//     printf("teleporting to x=%f, y=%f, z=%f\n", x, y, z);

//     memcpy(&x_bits, &x, sizeof(uint32_t));
//     memcpy(&y_bits, &y, sizeof(uint32_t));
//     memcpy(&z_bits, &z, sizeof(uint32_t));

//     ipc->Write<uint32_t>(0x189EA0, x_bits);
//     ipc->Write<uint32_t>(0x189EA4, y_bits);
//     ipc->Write<uint32_t>(0x189EA8, z_bits);

//     return 0;
// }

// int display_message(std::string message) {
//     std::shared_ptr<PCSX2> ipc = std::make_shared<PCSX2>();

//     if(ipc->Read<uint8_t>(0x1A8F00) != 0) {
//         std::cerr << "Cannot display message because the game is currently in
//         a paused state"; return -1;
//     }

//     std::optional<uint8_t*> possible_range = ipc_read_range(ipc,
//     (uint32_t)0x0, (uint32_t)0x2000000); if(!possible_range.has_value())
//     {return -1;}

//     std::optional<uint32_t> search = find_string(possible_range.value(),
//     0x2000000, "You got a skill point!\0\0G"); if(!search.has_value())
//     {return -1;} uint32_t msg_address = search.value();

//     // Overwrite end of "You got a skill point!" text with payload message.
//     std::string overwritten_text;
//     for(int i = 0; i <= message.length(); i++) {
//         overwritten_text += ipc->Read<uint8_t>(msg_address + i);
//         ipc->Write<uint8_t>(msg_address + i, message[i]);
//     }
//     overwritten_text += ipc->Read<uint8_t>(msg_address + message.length());
//     ipc->Write<uint8_t>(msg_address + message.length(), 0x0);

//     // Save original values for variables we use to trigger the text box.
//     uint8_t has_nice_ride = ipc->Read<uint8_t>(0x1A7A7D);
//     uint16_t ship_upgrades = ipc->Read<uint16_t>(0x1A7AF0);

//     // Set variables to trigger skill point get text box.
//     ipc->Write<uint8_t>(0x1A7A7D, 0);
//     ipc->Write<uint16_t>(0x1A7AF0, 0xFF50);

//     // After short delay, reset variables to original values.
//     std::this_thread::sleep_for(std::chrono::milliseconds(50));
//     ipc->Write<uint8_t>(0x1A7A7D, has_nice_ride);
//     ipc->Write<uint16_t>(0x1A7AF0, ship_upgrades);
//     for(int i = 0; i <= message.length(); i++) {
//         ipc->Write<uint8_t>(msg_address + i, overwritten_text[i]);
//     }

//     return 0;
// }

// int get_emulator_status() {
//     std::unique_ptr<PCSX2> ipc = std::make_unique<PCSX2>();
//     Shared::EmuStatus status = ipc->Status();

//     switch(status) {
//         case Shared::EmuStatus::Running:
//             std::cout << "running\n";
//             break;
//         case Shared::EmuStatus::Shutdown:
//             std::cout << "shutdown\n";
//             break;
//         case Shared::EmuStatus::Paused:
//             std::cout << "paused\n";
//             break;
//     }

//     return status;
// }

// int get_info() {
//     std::unique_ptr<PCSX2> ipc = std::make_unique<PCSX2>();

//     // For some reason the Version call only works after PCSX2 starts running
//     a game. printf("Platform: %s\n", ipc->Version());

//     // These calls also only work when PCSX2 is running a game but it makes
//     more sense for these calls. printf("Game: %s\n", ipc->GetGameTitle());
//     printf("ID: %s, Version: %s, UUID: %s\n", ipc->GetGameID(),
//     ipc->GetGameVersion(), ipc->GetGameUUID());

//     return 0;
// }

// std::optional<uint32_t> get_segment_table_address(std::shared_ptr<PCSX2> ipc)
// {
//     const uint8_t BEGINNING_OF_TABLE[8] = {0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x10, 0x00}; uint32_t* segment_pointer_table = nullptr;

//     std::optional<uint8_t*> range = ipc_read_range(ipc, 0x0, 0x2000000);
//     if(!range.has_value()) {return std::nullopt;}
//     uint32_t* ee_memory = (uint32_t*)range.value();

//     bool table_found = true;
//     for(int i = 0; i < EE_MEMORY_SIZE / 8; i++) {
//         if(!(ee_memory[i] == 0x0 && ee_memory[i + 1] == 0x100000)) {
//             continue;
//         }

//         for(int j = 2; j < Rac2::RAC2_SEGMENT_COUNT - 1; j++) {
//             if(ee_memory[i + j] > ee_memory[i + j + 1]) {
//                 table_found = false;
//                 break;
//             }
//         }

//         if(table_found) {
//             return i * 4;
//         }
//     }

//     std::cerr << "Could not find memory segment pointer table. You may not be
//     on a planet.\n"; return std::nullopt;
// }

// int print_memory_segments() {
//     std::shared_ptr<PCSX2> ipc = std::make_shared<PCSX2>();
//     std::optional<uint32_t> segment_table_address =
//     get_segment_table_address(ipc); if(!segment_table_address.has_value())
//     {return -1;}

//     std::optional<uint8_t*> range = ipc_read_range(ipc,
//     segment_table_address.value(), segment_table_address.value() +
//     sizeof(uint32_t) * RAC2_SEGMENT_COUNT); if(!range.has_value()) {return
//     -1;} uint32_t* segments = (uint32_t*)range.value();

//     for(int i = 0; i < RAC2_SEGMENT_COUNT; i++) {
//         printf("0x%08X - %s\n", segments[i],
//         Rac2::RAC2_SEGMENT_NAMES[i].c_str());
//     }

//     return 0;
// }

// int moby_info(int uid) {
//     std::shared_ptr<PCSX2> ipc = std::make_shared<PCSX2>();
//     std::optional<uint32_t> segment_table_address =
//     get_segment_table_address(ipc); if(!segment_table_address.has_value())
//     {return -1;}

//     std::optional<uint8_t*> range = ipc_read_range(ipc,
//     segment_table_address.value(), segment_table_address.value() +
//     sizeof(uint32_t) * RAC2_SEGMENT_COUNT); if(!range.has_value()) {return
//     -1;} uint32_t* segments = (uint32_t*)range.value();

//     uint32_t moby_segment_start = segments[22];
//     uint32_t moby_segment_end = segments[23] - 1;

//     ipc.reset();
//     return moby_info(moby_segment_start, moby_segment_end, uid);
// }

// int moby_info(uint32_t moby_segment_start, uint32_t moby_segment_end, int
// uid) {
//     std::shared_ptr<PCSX2> ipc = std::make_shared<PCSX2>();

//     uint32_t moby_address = moby_segment_start + uid * MOBY_SIZE;
//     if(moby_address > moby_segment_end) {
//         std::cerr << "Invalid Moby ID: The requested moby id was too
//         high.\n"; return -1;
//     }

//     printf("Moby Base Address: 0x%08X\n", moby_address);

//     uint32_t x = ipc->Read<uint32_t>(moby_address + 0x10);
//     uint32_t y = ipc->Read<uint32_t>(moby_address + 0x14);
//     uint32_t z = ipc->Read<uint32_t>(moby_address + 0x18);
//     // I don't want to talk about
//     it-----------------V------------V------------V printf("Position(0x10):
//     X=%f, Y=%f, Z=%f\n", *(float*)&x, *(float*)&y, *(float*)&z);
//     printf("State(0x20): %d\n", ipc->Read<uint8_t>(moby_address + 0x20));
//     printf("Group(0x21): %d\n", ipc->Read<uint8_t>(moby_address + 0x21));
//     printf("Class(0x22): %d\n", ipc->Read<uint8_t>(moby_address + 0x22));
//     printf("Alpha(0x23): %d\n", ipc->Read<uint8_t>(moby_address + 0x23));
//     printf("Moby Class Address(0x24): 0x%08X\n",
//     ipc->Read<uint32_t>(moby_address + 0x24)); printf("Chain Address(0x28):
//     0x%08X\n", ipc->Read<uint32_t>(moby_address + 0x28));

//     uint32_t scale = ipc->Read<uint32_t>(moby_address + 0x2C);
//     printf("Scale(0x2C): %f\n", *(float*)&scale);
//     printf("Drawn(0x31): %d\n", ipc->Read<uint8_t>(moby_address + 0x31));
//     printf("Draw Distance(0x32): %d\n", ipc->Read<uint16_t>(moby_address +
//     0x32)); std::bitset<16> flags1(ipc->Read<uint16_t>(moby_address + 0x34));
//     std::cout << "Flags 1(0x34): 0b" << flags1 << "\n";
//     std::bitset<16> flags2(ipc->Read<uint16_t>(moby_address + 0x36));
//     std::cout << "Flags 2(0x36): 0b" << flags2 << "\n";
//     printf("Lighting?(0x38): %d\n", ipc->Read<uint32_t>(moby_address +
//     0x38)); printf("Color(0x3C): R=%d, G=%d, B=%d, Shine=%d\n",
//     ipc->Read<uint8_t>(moby_address + 0x3C), ipc->Read<uint8_t>(moby_address
//     + 0x3D),
//                                                         ipc->Read<uint8_t>(moby_address
//                                                         + 0x3E),
//                                                         ipc->Read<uint8_t>(moby_address
//                                                         + 0x3F));

//     printf("Update Function Address(0x64): 0x%08X\n",
//     ipc->Read<uint32_t>(moby_address + 0x64)); printf("pVars Address(0x68):
//     0x%08X\n", ipc->Read<uint32_t>(moby_address + 0x68)); printf("collData
//     Address(0x98): 0x%08X\n", ipc->Read<uint32_t>(moby_address + 0x98));
//     printf("Collision Counter(0xA0): %d\n", ipc->Read<uint32_t>(moby_address
//     + 0xA0)); printf("oClass(0xAA): %d\n", ipc->Read<uint16_t>(moby_address +
//     0xAA)); printf("UID(0xB2): %d\n", ipc->Read<uint16_t>(moby_address +
//     0xB2));

//     return 0;
// }

// std::optional<uint8_t*> ipc_read_range(std::shared_ptr<PCSX2> ipc, uint32_t
// start, uint32_t end) {
//     assert((int32_t)(end - start) > 0);
//     //             VVVVV  MUST BE DIVISIBLE BY 8!
//     const int bytes_per_batch = 10000 * 8;
//     int memory_size = end - start;
//     int full_batches = memory_size / bytes_per_batch;
//     int remainder_batch_size = memory_size % bytes_per_batch;
//     remainder_batch_size += remainder_batch_size % 8;
//     // Since we read to ee_memory on 8 bytes chunks, add some buffer to the
//     end so the last read doesn't overrun. uint8_t* ee_memory = new
//     uint8_t[memory_size + memory_size % 8];

//     // Read all full batches
//     for (int i = 0; i < full_batches; i++) {
//         //printf("\nBatch Number: %d\n", i);
//         ipc->InitializeBatch();
//         for (int j = 0; j < bytes_per_batch; j += 8) {
//             int offset = i * bytes_per_batch + j;
//             // We read 64 bits at a time to reduce the amount of IPC reads
//             that we need to do. ipc->Read<uint64_t, true>(start + offset);
//         }
//         auto resr = ipc->FinalizeBatch();
//         ipc->SendCommand(resr);

//         for (int j = 0; j < bytes_per_batch / 8; j++) {
//             // Since the reply is in 64 bit chunks but we want to store it in
//             an array of bytes, we create
//             // a 64 bit pointer to the beginning of the byte array and copy
//             our 64 bit value into that. uint64_t* ee_mem_64 =
//             (uint64_t*)ee_memory; ee_mem_64[(i * (bytes_per_batch / 8) + j)]
//             = ipc->GetReply<PCSX2::MsgRead64>(resr, j);
//         }
//     }

//     // Read remaining partial batch
//     //printf("\nAppending remainder batch; size: %d\n",
//     remainder_batch_size); ipc->InitializeBatch(); for (int j = 0; j <
//     remainder_batch_size; j += 8) {
//         int offset = full_batches * bytes_per_batch + j;
//         // We read 64 bits at a time to reduce the amount of IPC reads that
//         we need to do. ipc->Read<uint64_t, true>(start + offset);
//     }
//     auto resr = ipc->FinalizeBatch();
//     ipc->SendCommand(resr);

//     for (int j = 0; j < remainder_batch_size / 8; j++) {
//         // Since the reply is in 64 bit chunks but we want to store it in an
//         array of bytes, we create
//         // a 64 bit pointer to the beginning of the byte array and copy our 64
//         bit value into that. uint64_t* ee_mem_64 = (uint64_t*)ee_memory;
//         ee_mem_64[(full_batches * (bytes_per_batch / 8) + j)] =
//         ipc->GetReply<PCSX2::MsgRead64>(resr, j);
//     }
//     return std::make_optional(ee_memory);
// }

// std::optional<uint32_t> find_byte_sequence(uint8_t* memory, size_t
// memory_size, const uint8_t* sequence, size_t seq_length) {
//     for(uint32_t i = 0; i < memory_size; i++){
//         if(memory[i] != sequence[0]){
//             continue;
//         }

//         if(memcmp(memory + i, sequence, seq_length) == 0){
//             return std::make_optional(i);
//         }

//     }

//     return std::nullopt;
// }

// std::optional<uint32_t> find_string(uint8_t* memory, size_t memory_size,
// std::string str) {
//     return find_byte_sequence(memory, memory_size, (uint8_t*)str.c_str(),
//     str.length());
// }