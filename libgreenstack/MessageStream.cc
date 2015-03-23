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
#include <algorithm>

#include <libgreenstack/MessageStream.h>
#include <libgreenstack/Message.h>
#include <libgreenstack/Buffer.h>
#include <libgreenstack/Frame.h>
#include "Reader.h"

void Greenstack::MessageStream::send(Greenstack::Message *message) {
    std::lock_guard<std::mutex> lock(sendMutex);
    Frame::encode(message, sendBytes, sendBytes.size());
}

Greenstack::Message *Greenstack::MessageStream::receive(void) {
    std::lock_guard<std::mutex> lock(recvMutex);
    size_t nbytes;

    Message *ret = Frame::create(recvBytes, 0, nbytes);
    if (ret != nullptr) {
        memmove(recvBytes.data(), recvBytes.data() + nbytes, recvBytes.size() - nbytes);
        recvBytes.resize(recvBytes.size() - nbytes);
    }

    return ret;
}

size_t Greenstack::MessageStream::drain(uint8_t *ptr, size_t size) {
    std::lock_guard<std::mutex> lock(sendMutex);
    size_t nbytes = std::min(size, sendBytes.size());
    memcpy(ptr, sendBytes.data(), nbytes);
    memmove(sendBytes.data(), sendBytes.data() + nbytes, sendBytes.size() - nbytes);
    sendBytes.resize(sendBytes.size() - nbytes);

    return nbytes;
}

size_t Greenstack::MessageStream::fill(const uint8_t *ptr, size_t sz) {
    std::lock_guard<std::mutex> lock(recvMutex);
    size_t offset = recvBytes.size();
    recvBytes.resize(recvBytes.size() + sz);
    memcpy(recvBytes.data() + offset, ptr, sz);

    return sz;
}
