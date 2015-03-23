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
    class AssumeRoleTest : public ::testing::Test {
    };

    TEST_F(AssumeRoleTest, AssumeRoleRequest) {
        AssumeRoleRequest request("myrole");
        EXPECT_STREQ("myrole", request.getRole().c_str());
        std::vector<uint8_t> data;
        Frame::encode(request, data, 0);

        auto msg = Frame::create(data);
        ASSERT_FALSE(msg == 0);

        auto decoded = dynamic_cast<AssumeRoleRequest*>(msg);
        ASSERT_FALSE(decoded == 0);
        EXPECT_STREQ(request.getRole().c_str(), decoded->getRole().c_str());
        delete decoded;
    }

    TEST_F(AssumeRoleTest, AssumeRoleRequestMissingRoleName) {
        EXPECT_ANY_THROW(new AssumeRoleRequest(""));
    }

    TEST_F(AssumeRoleTest, AssumeRoleResponse) {
        AssumeRoleResponse response;
        EXPECT_EQ(Greenstack::Status::Success, response.getStatus());

        std::vector<uint8_t> data;
        Frame::encode(response, data, 0);

        auto msg = Frame::create(data);
        ASSERT_FALSE(msg == 0);

        auto decoded = dynamic_cast<AssumeRoleResponse*>(msg);
        ASSERT_FALSE(decoded == 0);
        delete decoded;
    }
}
