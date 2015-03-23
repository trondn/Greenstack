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

#include <libgreenstack/visibility.h>
#include <vector>
#include <cstdint>
#include <cstring>
#include <stdexcept>

namespace Greenstack {
    class Buffer {
    public:
        virtual ~Buffer() {}

        /**
        * Get the pointer to the data area of this buffer. It has to be available
        * for the lifefime of the Buffer object.
        */
        virtual uint8_t *getData() const = 0;

        /**
        * Get the size of this buffer.
        */
        virtual const size_t getSize() const = 0;

        virtual void resize(size_t new_size) {
            if (supportsResize()) {
                throw std::runtime_error("Internal error: resize not implemented");
            } else {
                throw std::runtime_error("Buffer type does not support resize");
            }
        }

        virtual bool supportsResize() const {
            return false;
        }
    };

    class ByteArrayBuffer : public Buffer {
    public:
        ByteArrayBuffer() : Buffer() {
        }

        ByteArrayBuffer(const std::vector<uint8_t> &other) : Buffer() {
            buffer.resize(other.size());
            memcpy(buffer.data(), other.data(), other.size());
        }

        virtual ~ByteArrayBuffer() {}

        std::vector<uint8_t> &getBackend() {
            return buffer;
        }

        virtual uint8_t *getData() const override {
            return const_cast<uint8_t*>(buffer.data());
        }

        virtual const size_t getSize() const override {
            return buffer.size();
        }

        virtual void resize(size_t new_size) override {
            buffer.resize(new_size);
        }

        virtual bool supportsResize() const override {
            return true;
        }

    private:
        std::vector<uint8_t> buffer;
    };

    class FixedByteBuffer : public Buffer {
    public:
        FixedByteBuffer(uint8_t *ptr, size_t sz) : Buffer(), allocated(false), data(ptr), size(sz) {
            data = ptr;
        }

        FixedByteBuffer(size_t sz) : Buffer(), allocated(true), data(new uint8_t[sz]), size(sz) {
            data = new uint8_t[size];
        }

        virtual ~FixedByteBuffer() {
            if (allocated) {
                delete []data;
            }
        }

        virtual uint8_t *getData() const override {
            return data;
        }

        virtual const size_t getSize() const override {
            return size;
        }

    private:
        bool allocated;
        uint8_t *data;
        size_t size;
    };
}
