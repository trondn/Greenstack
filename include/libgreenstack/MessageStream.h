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
#include <mutex>
#include <libgreenstack/visibility.h>

namespace Greenstack {
    class Message;

    class LIBGREENSTACK_PUBLIC_API MessageStream {
    public:

        void send(Message *message);

        Message *receive(void);

        /* The "peer" should use the following API in order to
        * communicate with the MessageStream
         */
        size_t drain(uint8_t *ptr, size_t size);

        size_t fill(const uint8_t *ptr, size_t sz);


    private:
        std::mutex sendMutex;
        std::vector<uint8_t> sendBytes;

        std::mutex recvMutex;
        std::vector<uint8_t> recvBytes;
   };
}
