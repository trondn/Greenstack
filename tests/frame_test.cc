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
#undef NDEBUG
#include <assert.h>

#include <iostream>

static bool testEmptyRequest(void) {
    using namespace Greenstack;
    Request request;
    std::vector<uint8_t> vec;
    size_t nb = Frame::encode(request, vec);
    assert(nb == vec.size());

    std::vector<uint8_t> expected;
    // frame lenght
    expected.push_back(0x0);
    expected.push_back(0x0);
    expected.push_back(0x0);
    expected.push_back(0x7);

    // opaque
    expected.push_back(0xff);
    expected.push_back(0xff);
    expected.push_back(0xff);
    expected.push_back(0xff);

    // opcode
    expected.push_back(0xff);
    expected.push_back(0xff);

    // flags
    expected.push_back(0x0);

    assert(expected.size() == vec.size());
    assert(equal(vec.begin(), vec.end(), expected.begin()));

    return true;
}

static bool testEmptyResponse(void) {
    using namespace Greenstack;
    Response response;
    std::vector<uint8_t> vec;
    size_t nb = Frame::encode(response, vec);
    assert(nb == vec.size());

    std::vector<uint8_t> expected;
    // frame lenght
    expected.push_back(0x0);
    expected.push_back(0x0);
    expected.push_back(0x0);
    expected.push_back(0x9);

    // opaque
    expected.push_back(0xff);
    expected.push_back(0xff);
    expected.push_back(0xff);
    expected.push_back(0xff);

    // opcode
    expected.push_back(0xff);
    expected.push_back(0xff);

    // flags
    expected.push_back(0x1);

    // Status
    expected.push_back(0xff);
    expected.push_back(0xff);

    assert(expected.size() == vec.size());
    assert(equal(vec.begin(), vec.end(), expected.begin()));
    return true;
}

int main(void) {
    typedef bool (*testfunc)(void);
    std::map<std::string, testfunc> tests;
    int failed(0);

    tests["Empty request"] = testEmptyRequest;
    tests["Empty response"] = testEmptyResponse;

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