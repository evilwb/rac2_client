#pragma once

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

bool ap_connect(const std::string address, const std::string slotname, const std::string password = "");

inline moodycamel::ReaderWriterQueue<std::string> input_queue;
inline std::thread input_reader;
inline std::unique_ptr<Rac2::Rac2_interface> rac2;

inline bool rac2_connected;
inline bool ap_connected;
inline bool timeout;