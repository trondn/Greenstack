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

#include <cstddef>
#include <cstdint>
#include <vector>
#include <libgreenstack/visibility.h>
#include <libgreenstack/Reader.h>

namespace Greenstack {
    class Message;

    class LIBGREENSTACK_PUBLIC_API Frame {
    public:
        static size_t encode(const Message &message, std::vector<uint8_t> &vector, size_t offset = 0) {
            return encode(&message, vector, offset);
        }

        static size_t encode(const Message *message, std::vector<uint8_t> &vector, size_t offset = 0);

        /**
        * Create a Message from within the vector
        *
        * @param vector the data containing the Frame
        * @param offset where to start reading data
        * @param nbytes the number of bytes from the offset containing
        *               the complete frame.
        * @return the newly created message
        * @todo harden the function to validate the input and throw exception if its incorrect
        */
        static Message *create(const std::vector<uint8_t> &vector, size_t offset, size_t &nbytes) {
            ByteArrayReader reader(vector, offset);
            Message *ret = create(reader);
            nbytes = reader.getOffset() - offset;
            return ret;
        }

        static Message *create(const std::vector<uint8_t> &vector, size_t offset = 0) {
            size_t nbytes;
            return create(vector, offset, nbytes);
        }

        static Message *create(Reader &reader);
    };
}
