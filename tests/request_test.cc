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

#include <cstdlib>
#include <map>
#include <string>
#include <cstring>
#include <iostream>
#include <libgreenstack/Request.h>

#undef NDEBUG

#include <cassert>

using namespace Greenstack;


static bool constructor_test(void) {
    class RequestConstructorTest : public Request {
    public:
        RequestConstructorTest() : Request() {
            assert(sizeof(flags) == 1);
            uint8_t value;
            memcpy(&value, &flags, 1);
            assert(value == 0);
            assert(opaque == std::numeric_limits<uint32_t>::max());
            assert(opcode == std::numeric_limits<uint16_t>::max());
            assert(flexHeader.isEmpty());
        }
    } request;
    return true;
}

static bool empty_encode_test(void) {
    Request request;
    std::vector<uint8_t> data;
    request.encode(data);
    assert(data.size() == 7);
    // opaque 4 byes == 0xffffffff
    assert(data[0] == 0xff);
    assert(data[1] == 0xff);
    assert(data[2] == 0xff);
    assert(data[3] == 0xff);
    // opcode 2 bytes 0xffff
    assert(data[4] == 0xff);
    assert(data[5] == 0xff);
    // flags 1 byte 0
    assert(data[6] == 0x0);

    return true;
}

static bool getter_setter_test(void) {
    Request req;
    std::vector<uint8_t> data;

    req.setOpaque(0xaabbccdd);
    assert(req.getOpaque() == 0xaabbccdd);
    req.encode(data);
    assert(data[0] == 0xaa);
    assert(data[1] == 0xbb);
    assert(data[2] == 0xcc);
    assert(data[3] == 0xdd);

    req.setOpcode(0xeeff);
    assert(req.getOpcode() == 0xeeff);
    req.encode(data);
    assert(data[4] == 0xee);
    assert(data[5] == 0xff);

    req.setFenceBit(true);
    assert(req.isFenceBitSet());
    req.encode(data);
    assert(data[6] == 0x04);
    req.setFenceBit(false);
    assert(!req.isFenceBitSet());
    req.encode(data);
    assert(data[6] == 0x00);

    req.setMoreBit(true);
    assert(req.isMoreBitSet());
    req.encode(data);
    assert(data[6] == 0x08);
    req.setMoreBit(false);
    assert(!req.isMoreBitSet());
    req.encode(data);
    assert(data[6] == 0x00);

    req.setQuietBit(true);
    assert(req.isQuietBitSet());
    req.encode(data);
    assert(data[6] == 0x10);
    req.setQuietBit(false);
    assert(!req.isQuietBitSet());
    req.encode(data);
    assert(data[6] == 0x00);

    std::vector<uint8_t> payload;
    payload.resize(20);
    req.setPayload(payload);
    req.encode(data);
    assert(data.size() == 27);
    auto iterator = data.begin();
    iterator += 7;
    assert(equal(iterator, data.end(), payload.begin()));

    req.setFenceBit(true);
    req.setMoreBit(true);
    req.setQuietBit(true);
    assert(req.isQuietBitSet());
    req.encode(data);
    assert(data[6] == 0x1c);

    req.getFlexHeader().setHash(32);
    req.encode(data);
    assert(data[6] == 0x1e); // is causing the flex header bit to be added

    return true;
}

bool decode_test(void) {
    class RequestEncodeTest : public Request {
    public:
        RequestEncodeTest() : Request() {
            std::vector<uint8_t> payload;
            payload.resize(20);
            setPayload(payload);

            setOpaque(0xaabbccdd);
            setOpcode(0xeeff);
            setFenceBit(true);
            setMoreBit(true);
            setQuietBit(true);
            getFlexHeader().setHash(32);
        }

        bool equals(Message &other) {
            assert(other.getOpaque() == 0xaabbccdd);
            assert(other.getOpcode() == 0xeeff);
            assert(other.isFenceBitSet());
            assert(other.isMoreBitSet());
            assert(other.isQuietBitSet());
            assert(other.getFlexHeader().getHash() == 32);
            return true;
        }
    } request;

    std::vector<uint8_t> data;
    request.encode(data);

    auto restored = Message::create(data);
    assert(dynamic_cast<Request*>(restored));
    assert(request.equals(*restored));
    delete restored;

    return true;
}

int main(void) {
    typedef bool (*testfunc)(void);
    std::map<std::string, testfunc> tests;
    int failed(0);

    tests["constructor"] = constructor_test;
    tests["empty encode"] = empty_encode_test;
    tests["getter and setter"] = getter_setter_test;
    tests["decode"] = decode_test;

    for (auto iter = tests.begin(); iter != tests.end(); ++iter) {
        std::cout << iter->first << "... ";
        std::cout.flush();
        if (iter->second()) {
            std::cout << "ok" << std::endl;
        } else {
            ++failed;
            // error should already be printed
        }
    }

    if (failed) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}
