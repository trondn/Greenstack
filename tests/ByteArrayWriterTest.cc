/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
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
#include <iostream>
#include <iomanip>

#include "../libgreenstack/Writer.h"

using namespace Greenstack;

int main(void) {
    ByteArrayBuffer buffer;
    BufferWriter writer(buffer);

    writer.write((uint8_t)0xde);
    if (buffer.getSize() != 1) {
        std::cerr << "Invalid size after writing a byte"<< std::endl;
        exit(EXIT_FAILURE);
    }
    writer.write((uint16_t)0xdead);
    if (buffer.getSize() != 3) {
        std::cerr << "Invalid size after writing uint16_t"<< std::endl;
        exit(EXIT_FAILURE);
    }

    writer.write((uint32_t)0xdeadbeef);
    if (buffer.getSize() != 7) {
        std::cerr << "Invalid size after writing uint32_t" << std::endl;
        exit(EXIT_FAILURE);
    }

    uint64_t val = 0xdeadbeef;
    val <<= 32;
    val |= 0xdeadcafe;

    writer.write(val);
    if (buffer.getSize() != 15) {
        std::cerr << "Invalid size after writing uint64_t" << std::endl;
        exit(EXIT_FAILURE);
    }

    uint8_t expected[15] = { 0xde, 0xde, 0xad, 0xde, 0xad, 0xbe, 0xef,
                             0xde, 0xad, 0xbe, 0xef, 0xde, 0xad, 0xca, 0xfe };

    for (int ii = 1; ii < sizeof(expected); ++ii) {;
        if (expected[ii] != buffer.getData()[ii]) {
            std::cerr << "Incorrect value at offset " << ii << " expected "
                    << std::hex << (unsigned int)expected[ii] << " got "
                    << std::hex << (unsigned int)buffer.getData()[ii]
                    << std::endl;
            exit(EXIT_FAILURE);
        }

    }



    return EXIT_SUCCESS;
}
