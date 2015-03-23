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
#pragma once

#include <vector>
#include <limits>
#include <cstring>
#include <stdexcept>

#include "config.h"

namespace Greenstack {
    /**
     * The ByteArrayReader allows for reading values from a byte array
     * in a host specific byte order.
     */
    class ByteArrayReader {
    public:
        ByteArrayReader(const std::vector<uint8_t> &vec, size_t start = 0, size_t end = std::numeric_limits<size_t>::max())
                : ByteArrayReader(vec.data(), start, end) {

            // Fixup end marker
            if (end > vec.size()) {
                size = vec.size() - start;
            }
        }

        ByteArrayReader(const uint8_t *ptr, size_t start, size_t end)
                : data(ptr + start), offset(0), size(end - start) {
        }

        ByteArrayReader(const uint8_t *ptr, size_t end)
                : data(ptr), offset(0), size(end) {
        }

        void skip(size_t nbytes = 1) {
            offset += nbytes;
        }

        void reset(void) {
            offset = 0;
        }

        void read(uint8_t &val) {
            pread(val, offset);
            offset += sizeof(val);
        }

        void read(uint16_t &val) {
            pread(val, offset);
            offset += sizeof(val);
        }

        void read(uint32_t &val) {
            pread(val, offset);
            offset += sizeof(val);
        }

        void read(uint64_t &val) {
            pread(val, offset);
            offset += sizeof(val);
        }

        void read(uint8_t *ptr, size_t len) {
            pread(ptr, len, offset);
            offset += len;
        }

        size_t getOffset(void) const {
            return offset;
        }

        void pread(uint8_t &val, size_t off) const {
            pread(&val, sizeof(val), off);
        }

        void pread(uint16_t &val, size_t off) const {
            uint16_t value;
            pread(reinterpret_cast<uint8_t *>(&value), sizeof(value), off);
            val = ntohs(value);
        }

        void pread(uint32_t &val, size_t off) const {
            uint32_t value;
            pread(reinterpret_cast<uint8_t *>(&value), sizeof(value), off);
            val = ntohl(value);
        }

        void pread(uint64_t &val, size_t off) const {
            uint64_t value;
            pread(reinterpret_cast<uint8_t *>(&value), sizeof(value), off);
            val = ntohll(value);
        }

        void pread(uint8_t *ptr, size_t len, size_t off) const {
            if ((off + len) > size) {
                throw std::runtime_error("Access outside buffer");
            }
            memcpy(ptr, data + off, len);
        }

        size_t getReminder(void) {
            return size - offset;
        }

    private:
        const uint8_t *data;
        size_t offset;
        size_t size;
    };
}
