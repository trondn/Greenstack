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
#include <libgreenstack/Frame.h>
#include <libgreenstack/Message.h>
#include <cassert>

#include "Writer.h"
#include "Reader.h"


size_t Greenstack::Frame::encode(const Message *message, std::vector<uint8_t> &vector, size_t offset)  {
    ByteArrayWriter writer(vector, offset);
    size_t nb = message->encode(vector, offset + 4);
    writer.write(static_cast<uint32_t>(nb));
    return nb + 4;
}

Greenstack::Message *Greenstack::Frame::create(const std::vector<uint8_t> &vector, size_t offset, size_t &nbytes) {
    assert(offset < vector.size());
    size_t currsize = vector.size();

    if (currsize < 4) {
        // we need the header size...
        return nullptr;
    }

    ByteArrayReader reader(vector, offset);
    uint32_t size;
    reader.read(size);

    if (currsize < (size + 4)) {
        // we need more data
        return nullptr;
    }

    nbytes = size + 4;
    return Message::create(vector, offset + 4, offset + nbytes);
}
