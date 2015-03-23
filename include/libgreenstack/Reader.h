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
#include <stddef.h>
#include <iostream>
#include <cassert>

#include "config.h"
#include "Buffer.h"

namespace Greenstack {
    class Reader {
    public:
        Reader(size_t off, size_t sz) : offset(off), size(sz) {

        }

        virtual ~Reader() {

        }

        void seek(int nbytes) {
            offset += nbytes;
        }

        void skip(size_t nbytes = 1) {
            offset += nbytes;
        }

        void reset() {
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

        size_t getOffset() const {
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

        virtual void pread(uint8_t *ptr, size_t len, size_t off) const = 0;

        size_t getRemainder() const {
            return size - offset;
        }

        size_t getSize() const {
            return size;
        }

    protected:
        void guard(size_t off, size_t len) const {
            if ((off + len) > size) {
                throw std::runtime_error("Access outside buffer");
            }
        }

        size_t offset;
        size_t size;
    };

    class ByteArrayReader : public Reader {
    public:
        ByteArrayReader(const uint8_t *ptr, size_t off, size_t len) : Reader(off, len), data(ptr) {
        }

        ByteArrayReader(const Greenstack::Buffer &buff, size_t off = 0) : Reader(off, buff.getSize()), data(buff.getData()) {
        }

        ByteArrayReader(const std::vector<uint8_t> &vec) : Reader(0, vec.size()), data(vec.data()) {

        }

        ByteArrayReader(const std::vector<uint8_t> &vec, size_t off) : Reader(off, vec.size()), data(vec.data()) {

        }

        ByteArrayReader(const std::vector<uint8_t> &vec, size_t off, size_t end) : Reader(off, end), data(vec.data()) {
            assert(end <= vec.size());
        }

        virtual void pread(uint8_t *ptr, size_t len, size_t off) const {
            guard(off, len);
            memcpy(ptr, data + off, len);
        }
    protected:
        const uint8_t *data;
    };
}
