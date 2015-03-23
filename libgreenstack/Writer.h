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
#pragma once

#include <libgreenstack/Buffer.h>
#include "config.h"

#include <iostream>

namespace Greenstack {

    class Writer {
    public:
        Writer(size_t off) : offset(off) {

        }

        virtual ~Writer() {

        }

        void write(uint8_t val) {
            pwrite(val, offset);
            offset += sizeof(val);
        }

        void write(uint16_t val) {
            pwrite(val, offset);
            offset += sizeof(val);
        }

        void write(uint32_t val) {
            pwrite(val, offset);
            offset += sizeof(val);
        }

        void write(uint64_t val) {
            pwrite(val, offset);
            offset += sizeof(val);
        }

        void write(const uint8_t *ptr, size_t len) {
            pwrite(ptr, len, offset);
            offset += len;
        }

        size_t getOffset(void) const {
            return offset;
        }

        void pwrite(uint8_t value, size_t off) {
            pwrite(reinterpret_cast<uint8_t *>(&value), sizeof(value), off);
        }

        void pwrite(uint16_t val, size_t off) {
            uint16_t value = htons(val);
            pwrite(reinterpret_cast<uint8_t *>(&value), sizeof(value), off);
        }

        void pwrite(uint32_t val, size_t off) {
            uint32_t value = htonl(val);
            pwrite(reinterpret_cast<uint8_t *>(&value), sizeof(value), off);
        }

        void pwrite(uint64_t val, size_t off) {
            uint64_t value = htonll(val);
            pwrite(reinterpret_cast<uint8_t *>(&value), sizeof(value), off);
        }

        virtual void pwrite(const uint8_t *ptr, size_t len, size_t off) = 0;

        void skip(size_t nbytes = 1) {
            offset += nbytes;
        }

    protected:
        size_t offset;
    };

    class ByteArrayWriter : public Writer {
    public:
        ByteArrayWriter(std::vector<uint8_t> &vec, size_t start = 0) : Writer(0), startOffset(start), vector(vec) {
                // EMPTY
        }

        virtual void pwrite(const uint8_t *ptr, size_t len, size_t off) {
            if ((startOffset + off + len) > vector.size()) {
                vector.resize(startOffset + off + len);
            }
            memcpy(vector.data() + off + startOffset, ptr, len);
        }

    private:
        const size_t startOffset;
        std::vector<uint8_t> &vector;
    };

    class BufferWriter : public Writer {
    public:
        BufferWriter(Buffer &buff, size_t start = 0) : Writer(0), startOffset(start), buffer(buff) {
            // EMPTY
        }

        virtual void pwrite(const uint8_t *ptr, size_t len, size_t off) {
            if ((startOffset + off + len) > buffer.getSize()) {
                // May throw exception if the underlying buffer don't support resize
                buffer.resize(startOffset + off + len);
            }
            memcpy(buffer.getData() + off + startOffset, ptr, len);
        }

    private:
        const size_t startOffset;
        Buffer &buffer;
    };

}
