/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
/*
 *     Copyright 2015 Couchbase, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
#include <cstdlib>
#include <map>
#include <string>
#include <iostream>

#include "../libgreenstack/Reader.h"

using namespace Greenstack;

int main(void) {
    std::vector<uint8_t> data;

    // write in 0xdeadfeefcafeed
    data.push_back(0xde);
    data.push_back(0xad);
    data.push_back(0xbe);
    data.push_back(0xef);
    data.push_back(0xca);
    data.push_back(0xfe);
    data.push_back(0xfe);
    data.push_back(0xed);

    ByteArrayReader reader(data);

    // We should be able to seek 8 times
    for (auto ii = data.size(); ii > 0; --ii) {
        if (reader.getReminder() != ii) {
            std::cerr << "Error: expected "
                    << reader.getReminder()
                    << " to return " << ii << std::endl;
            return EXIT_FAILURE;
        }
        reader.skip();
    }

    reader.reset();
    if (reader.getReminder() != data.size()) {
        std::cerr << "Error: expected " << reader.getReminder()
                << " to return " << data.size() << " after reset"
                << std::endl;
        return EXIT_FAILURE;
    }

    // We should be able to seek more than a single byte
    for (auto ii = data.size(); ii > 0; ii -= 2) {
        if (reader.getReminder() != ii) {
            std::cerr << "Error: expected "
                    << reader.getReminder()
                    << " to return " << ii << std::endl;
            return EXIT_FAILURE;
        }
        reader.skip(2);
    }
    reader.reset();

    // We should be able to read single bytes
    for (auto ii = data.size(); ii > 0; --ii) {
        uint8_t u8;
        reader.read(u8);
        if (data[data.size() - ii] != u8) {
            std::cerr << "Error: wrong value returned" << std::endl;
            return EXIT_FAILURE;
        }
    }

    // trying to read another byte should throw exception
    try {
        uint8_t u8;
        reader.read(u8);
        std::cerr << "Error: Should not be able to read outside buffer" << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        // success
    }

    reader.reset();
    uint16_t u16[4];
    for (int ii = 0; ii < 4; ++ii) {
        reader.read(u16[ii]);
    }
    if (u16[0] != 0xdead || u16[1] != 0xbeef ||
            u16[2] != 0xcafe || u16[3] != 0xfeed) {
        std::cerr << "Error uint16 read failure" << std::endl;
        return EXIT_FAILURE;
    }

    reader.reset();
    uint32_t u32[2];
    for (int ii = 0; ii < 2; ++ii) {
        reader.read(u32[ii]);
    }
    if (u32[0] != 0xdeadbeef || u32[1] != 0xcafefeed) {
        std::cerr << "Error uint32 read failure" << std::endl;
        return EXIT_FAILURE;
    }

    reader.reset();
    uint64_t u64;
    reader.read(u64);
    if (u64 != 0xdeadbeefcafefeed) {
        std::cerr << "Error uint64 read failure" << std::endl;
        return EXIT_FAILURE;
    }

    // It should be possible to create from a char array
    ByteArrayReader reader1(data.data(), 1, 3);
    reader1.read(u16[0]);
    if (u16[0] != 0xadbe) {
        std::cerr << "Error uint16 read failure when initialized from array" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
