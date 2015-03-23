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
    class LIBGREENSTACK_PUBLIC_API Buffer {
    public:
        virtual ~Buffer() {}

        /**
        * Get the pointer to the data area of this buffer. It has to be available
        * for the lifefime of the Buffer object.
        */
        virtual uint8_t *getData(void) const = 0;

        /**
        * Get the size of this buffer.
        */
        virtual const size_t getSize(void) const = 0;

        virtual void resize(size_t new_size) {
            if (supportsResize()) {
                throw std::runtime_error("Internal error: resize not implemented");
            } else {
                throw std::runtime_error("Buffer type does not support resize");
            }
        }

        virtual bool supportsResize(void) const {
            return false;
        }
    };

    class LIBGREENSTACK_PUBLIC_API ByteArrayBuffer : public Buffer {
    public:
        ByteArrayBuffer() : Buffer() {
        }

        ByteArrayBuffer(const std::vector<uint8_t> &other) : Buffer() {
            buffer.resize(other.size());
            memcpy(buffer.data(), other.data(), other.size());
        }

        virtual ~ByteArrayBuffer() {}

        std::vector<uint8_t> &getBackend(void) {
            return buffer;
        }

        virtual uint8_t *getData(void) const {
            return const_cast<uint8_t*>(buffer.data());
        }

        virtual const size_t getSize(void) const {
            return buffer.size();
        }

        virtual void resize(size_t new_size) {
            buffer.resize(new_size);
        }

        virtual bool supportsResize(void) const {
            return true;
        }

    private:
        std::vector<uint8_t> buffer;
    };

    class LIBGREENSTACK_PUBLIC_API FixedByteBuffer : public Buffer {
    public:
        FixedByteBuffer(size_t size) : Buffer() {
            data = new uint8_t[size];
        }

        virtual ~FixedByteBuffer() {
            delete []data;
        }

        virtual uint8_t *getData(void) const {
            return data;
        }

        virtual const size_t getSize(void) const {
            return size;
        }

    private:
        uint8_t *data;
        size_t size;
    };
}
