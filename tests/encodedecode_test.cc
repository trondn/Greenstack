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
#include <libgreenstack/Greenstack.h>
#include <libgreenstack/Frame.h>
#include <cassert>
#include <mutex>

namespace Greenstack {

    class Message;

    class MessageQueue {
    public:
        MessageQueue();
        ~MessageQueue();

        bool empty(void);

        Message *pop(void);
        void push(Message &message);

        void push(Message *message) {
            push(*message);
        }

    private:
        MessageQueue(MessageQueue &) = delete;

        std::mutex mutex;
        std::vector<uint8_t> data;
        size_t offset;
    };
}


Greenstack::MessageQueue::MessageQueue() : offset(0) {
}

Greenstack::MessageQueue::~MessageQueue() {
}

bool Greenstack::MessageQueue::empty(void) {
    std::lock_guard<std::mutex> lock(mutex);
    return data.empty();
}

Greenstack::Message *Greenstack::MessageQueue::pop(void) {
    if (empty()) {
        return 0;
    }

    std::lock_guard<std::mutex> lock(mutex);
    size_t nbytes;
    Message *ret = Frame::create(data, offset, nbytes);
    offset += nbytes;
    if (offset == data.size()) {
        data.resize(0);
        offset = 0;
    }

    return ret;
}

void Greenstack::MessageQueue::push(Greenstack::Message &message) {
    std::lock_guard<std::mutex> lock(mutex);
    Frame::encode(message, data, data.size());
}

int main(void) {
    Greenstack::MessageQueue mq;
    Greenstack::Message *request = new Greenstack::Request();
    Greenstack::Message *response = new Greenstack::Response();

    assert(mq.empty());
    assert(mq.pop() == 0);

    mq.push(request);
    mq.push(response);

    delete request;
    delete response;

    request = mq.pop();
    response = mq.pop();

    assert(dynamic_cast<Greenstack::Request*>(request));
    assert(dynamic_cast<Greenstack::Response*>(response));

    delete request;
    delete response;

    assert(mq.pop() == NULL);

    return EXIT_SUCCESS;
}