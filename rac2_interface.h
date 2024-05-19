#pragma once

#include <memory>
#include <string>
#include <optional>
#include <unordered_map>
#include <vector>

#include "pine/pine.h"

namespace Rac2 {

const uint32_t RAC2_SEGMENT_COUNT = 35;
const uint32_t EE_MEMORY_SIZE = 0x2000000;

const std::string RAC2_SEGMENT_NAMES[RAC2_SEGMENT_COUNT] = {
    "OS",
    "Code",
    "Base",
    "",
    "",
    "",
    "",
    "Tfrag Geometry",
    "Occlusion",
    "Sky",
    "Collision",
    "Shared VRAM",
    "Particle VRAM",
    "Effects VRAM",
    "Mobies",
    "Ties",
    "Shrubs",
    "Ratchet Seqs",
    "",
    "Help Messages",
    "Tie Instances",
    "Shrub Instances",
    "Moby Instances",
    "Moby Pvars",
    "Misc Instances",
    "",
    "",
    "",
    "",
    "",
    "",
    "HUD",
    "GUI",
    "",
    ""};

class Rac2_interface {
public:
    struct Item {
        std::string name;
        uint32_t address;
    };

    Rac2_interface();
    bool connect();
    int give_item(Item);
    const std::vector<Item> *list_inventory();
    int display_message(std::string);

    const std::unordered_map<uint32_t, Item> item_table = {
        {1, Item{"Lancer", 0x1A7B16}},
        {2, Item{"Lava Gun", 0x1A7B15}}
    };

    std::string platform;
    std::string title;
    std::string id;
    std::string version;
    std::string uuid;

private:
    int get_emulator_status();
    int get_info();
    int get_pause_state();
    int get_current_planet();
    int get_ratchet_position();
    int set_ratchet_position(float, float, float);
    std::optional<uint32_t> get_segment_table_address();
    int print_memory_segments();
    int moby_info(int);
    int moby_info(uint32_t, uint32_t, int);
    std::optional<uint8_t *> ipc_read_range(std::shared_ptr<PINE::PCSX2>, uint32_t, uint32_t);
    std::optional<uint32_t> find_byte_sequence(uint8_t *, size_t, const uint8_t *, size_t);
    std::optional<uint32_t> find_string(uint8_t *, size_t, std::string);

    std::shared_ptr<PINE::PCSX2> ipc;
};

} // namespace Rac2