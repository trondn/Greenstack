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
#include <gtest/gtest.h>

namespace Greenstack {
    class SelectBucketTest : public ::testing::Test {
    };

    TEST_F(SelectBucketTest, SelectBucketRequest) {
        SelectBucketRequest request("bucket");
        EXPECT_STREQ("bucket", request.getName().c_str());
        std::vector<uint8_t> data;
        Frame::encode(request, data, 0);

        auto msg = Frame::create(data);
        ASSERT_FALSE(msg == 0);

        auto decoded = dynamic_cast<SelectBucketRequest*>(msg);
        ASSERT_FALSE(decoded == 0);
        EXPECT_STREQ(request.getName().c_str(), decoded->getName().c_str());
        delete decoded;
    }

    TEST_F(SelectBucketTest, SelectBucketRequestMissingBucketName) {
        EXPECT_ANY_THROW(new SelectBucketRequest(""));
    }

    TEST_F(SelectBucketTest, SelectBucketResponse) {
        SelectBucketResponse response;
        EXPECT_EQ(Greenstack::Status::Success, response.getStatus());

        std::vector<uint8_t> data;
        Frame::encode(response, data, 0);

        auto msg = Frame::create(data);
        ASSERT_FALSE(msg == 0);

        auto decoded = dynamic_cast<SelectBucketResponse*>(msg);
        ASSERT_FALSE(decoded == 0);
        delete decoded;
    }
}
