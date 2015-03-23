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
    class FrameTest : public ::testing::Test {
    };

    TEST_F(FrameTest, EncodeEmptyRequest) {
        Greenstack::Request request;
        std::vector<uint8_t> vec;
        size_t nb = Greenstack::Frame::encode(request, vec);
        EXPECT_EQ(nb, vec.size());

        std::vector<uint8_t> expected;
        // frame length
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

        EXPECT_EQ(expected.size(), vec.size());
        EXPECT_TRUE(equal(vec.begin(), vec.end(), expected.begin()));
    }

    TEST_F(FrameTest, EncodeEmptyResponse) {
        Greenstack::Response response;
        std::vector<uint8_t> vec;
        size_t nb = Greenstack::Frame::encode(response, vec);
        EXPECT_EQ(nb, vec.size());

        std::vector<uint8_t> expected;
        // frame length
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
        expected.push_back(0x01);

        // Status
        expected.push_back(0xff);
        expected.push_back(0xff);

        EXPECT_EQ(expected.size(), vec.size());
        EXPECT_TRUE(equal(vec.begin(), vec.end(), expected.begin()));
    }

    TEST_F(FrameTest, DecodeNoData) {
        std::vector<uint8_t> data;
        EXPECT_TRUE(Frame::create(data) == 0);
    }

    TEST_F(FrameTest, DecodeInsufficientData) {
        std::vector<uint8_t> data;
        ByteArrayWriter writer(data);
        writer.write((uint32_t)10);
        EXPECT_TRUE(Frame::create(data) == 0);
    }

    TEST_F(FrameTest, DecodeResponseWithFlags) {
        std::vector<uint8_t> data;
        // frame length
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x9);

        // opaque
        data.push_back(0xaa);
        data.push_back(0xbb);
        data.push_back(0xcc);
        data.push_back(0xdd);

        // opcode
        data.push_back(0xaa);
        data.push_back(0xdd);

        // flags
        data.push_back(0x1d);

        // Status
        data.push_back(0xfe);
        data.push_back(0xfe);

        auto decoded = Frame::create(data);
        ASSERT_FALSE(decoded == 0);
        EXPECT_EQ(0xaabbccdd, decoded->getOpaque());
        EXPECT_EQ(0xaadd, decoded->getOpcode());

        auto response = dynamic_cast<Response*>(decoded);
        ASSERT_FALSE(response == 0);
        EXPECT_EQ(0xfefe, response->getStatus());
        EXPECT_TRUE(response->getFlexHeader().isEmpty());
        EXPECT_TRUE(response->isFenceBitSet());
        EXPECT_TRUE(response->isMoreBitSet());
        EXPECT_TRUE(response->isQuietBitSet());

        delete decoded;
    }

    TEST_F(FrameTest, DecodeResponseWithUnassignedFlags) {
        std::vector<uint8_t> data;
        // frame length
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x9);

        // opaque
        data.push_back(0xaa);
        data.push_back(0xbb);
        data.push_back(0xcc);
        data.push_back(0xdd);

        // opcode
        data.push_back(0xaa);
        data.push_back(0xdd);

        // flags
        data.push_back(0x71);

        // Status
        data.push_back(0xfe);
        data.push_back(0xfe);

        EXPECT_ANY_THROW(Frame::create(data));
    }

    TEST_F(FrameTest, DecodeResponseWithNextFlag) {
        std::vector<uint8_t> data;
        // frame length
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x9);

        // opaque
        data.push_back(0xaa);
        data.push_back(0xbb);
        data.push_back(0xcc);
        data.push_back(0xdd);

        // opcode
        data.push_back(0xaa);
        data.push_back(0xdd);

        // flags
        data.push_back(0x81);

        // Status
        data.push_back(0xfe);
        data.push_back(0xfe);

        EXPECT_ANY_THROW(Frame::create(data));
    }


    TEST_F(FrameTest, DecodeResponse) {
        std::vector<uint8_t> data;
        // frame length
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x9);

        // opaque
        data.push_back(0xaa);
        data.push_back(0xbb);
        data.push_back(0xcc);
        data.push_back(0xdd);

        // opcode
        data.push_back(0xaa);
        data.push_back(0xdd);

        // flags
        data.push_back(0x01);

        // Status
        data.push_back(0xfe);
        data.push_back(0xfe);

        auto decoded = Frame::create(data);
        ASSERT_FALSE(decoded == 0);
        EXPECT_EQ(0xaabbccdd, decoded->getOpaque());
        EXPECT_EQ(0xaadd, decoded->getOpcode());

        auto response = dynamic_cast<Response*>(decoded);
        ASSERT_FALSE(response == 0);
        EXPECT_EQ(0xfefe, response->getStatus());
        EXPECT_TRUE(response->getFlexHeader().isEmpty());
        EXPECT_FALSE(response->isFenceBitSet());
        EXPECT_FALSE(response->isMoreBitSet());
        EXPECT_FALSE(response->isQuietBitSet());

        delete decoded;
    }


    TEST_F(FrameTest, DecodeRequest) {
        std::vector<uint8_t> data;
        // frame length
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x7);

        // opaque
        data.push_back(0xff);
        data.push_back(0xee);
        data.push_back(0xdd);
        data.push_back(0xcc);

        // opcode
        data.push_back(0xbb);
        data.push_back(0xaa);

        // flags
        data.push_back(0x0);

        auto decoded = Frame::create(data);
        ASSERT_FALSE(decoded == 0);
        EXPECT_EQ(0xffeeddcc, decoded->getOpaque());
        EXPECT_EQ(0xbbaa, decoded->getOpcode());

        auto request = dynamic_cast<Request*>(decoded);
        ASSERT_FALSE(request == 0);
        EXPECT_TRUE(request->getFlexHeader().isEmpty());
        EXPECT_FALSE(request->isFenceBitSet());
        EXPECT_FALSE(request->isMoreBitSet());
        EXPECT_FALSE(request->isQuietBitSet());

        delete decoded;
    }


    TEST_F(FrameTest, DecodeRequestWithUnassignedFlags) {
        std::vector<uint8_t> data;
        // frame length
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x9);

        // opaque
        data.push_back(0xaa);
        data.push_back(0xbb);
        data.push_back(0xcc);
        data.push_back(0xdd);

        // opcode
        data.push_back(0xaa);
        data.push_back(0xdd);

        // flags
        data.push_back(0x70);

        // Status
        data.push_back(0xfe);
        data.push_back(0xfe);

        EXPECT_ANY_THROW(Frame::create(data));
    }

    TEST_F(FrameTest, DecodeRequestWithNextFlag) {
        std::vector<uint8_t> data;
        // frame length
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x9);

        // opaque
        data.push_back(0xaa);
        data.push_back(0xbb);
        data.push_back(0xcc);
        data.push_back(0xdd);

        // opcode
        data.push_back(0xaa);
        data.push_back(0xdd);

        // flags
        data.push_back(0x80);

        // Status
        data.push_back(0xfe);
        data.push_back(0xfe);

        EXPECT_ANY_THROW(Frame::create(data));
    }

    TEST_F(FrameTest, DecodeRequestWithFlags) {
        std::vector<uint8_t> data;
        // frame length
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x0);
        data.push_back(0x7);

        // opaque
        data.push_back(0xaa);
        data.push_back(0xbb);
        data.push_back(0xcc);
        data.push_back(0xdd);

        // opcode
        data.push_back(0xaa);
        data.push_back(0xdd);

        // flags
        data.push_back(0x1c);

        auto decoded = Frame::create(data);
        ASSERT_FALSE(decoded == 0);
        EXPECT_EQ(0xaabbccdd, decoded->getOpaque());
        EXPECT_EQ(0xaadd, decoded->getOpcode());

        EXPECT_TRUE(decoded->getFlexHeader().isEmpty());
        EXPECT_TRUE(decoded->isFenceBitSet());
        EXPECT_TRUE(decoded->isMoreBitSet());
        EXPECT_TRUE(decoded->isQuietBitSet());

        delete decoded;
    }

}
