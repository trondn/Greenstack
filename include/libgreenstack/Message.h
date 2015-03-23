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

#include <limits>
#include <vector>
#include <cstdint>
#include <libgreenstack/visibility.h>
#include <libgreenstack/FlexHeader.h>
#include <libgreenstack/Opcodes.h>
#include <libgreenstack/Reader.h>
#include <ostream>

namespace Greenstack {
    class ByteArrayWriter;

    class LIBGREENSTACK_PUBLIC_API Message {
    public:
        static Message *create(const std::vector<uint8_t> &vector, size_t offset = 0) {
            return create(vector, offset, vector.size());
        }
        static Message *create(const std::vector<uint8_t> &vector, size_t offset, size_t end) {
            ByteArrayReader reader(vector, offset, end);
            return create(reader, end);
        }

        static Message *create(Reader &reader, size_t size);

        virtual ~Message();

        void setOpaque(uint32_t value);
        uint32_t getOpaque() const;

        void setOpcode(opcode_t value);
        opcode_t getOpcode() const;

        void setFenceBit(bool enable);
        bool isFenceBitSet() const;

        void setMoreBit(bool enable);
        bool isMoreBitSet() const;

        void setQuietBit(bool enable);
        bool isQuietBitSet() const;

        Greenstack::FlexHeader &getFlexHeader();
        const Greenstack::FlexHeader &getFlexHeader() const;

        /**
        * Set the payload for the message
        */
        void setPayload(std::vector<uint8_t> &data);

        /**
        * Encode the content of this Request into the provided
        * vector at the given offset. Encode may call resize() in
        * order to grow the buffer to fit the data. This <em>may</em>
        * invalidate any pointers previously held pointing into
        * the buffer.
        *
        * @param vector the destination for the encoded packet
        * @param offset the offset inside the vector to encode the
        *               request
        * @return the number of bytes the encoded object occupied
        */
        virtual size_t encode(std::vector<uint8_t> &vector, size_t offset = 0) const;

    protected:
        Message(bool response);

        /**
        * validate that the payload is correct. Throw an exception otherwise
        */
        virtual void validate();

        uint32_t opaque;
        opcode_t opcode;

        struct Flags {
            bool response : 1;
            bool flexHeaderPresent : 1;
            bool fence : 1;
            bool more : 1;
            bool quiet : 1;
            bool unassigned : 2;
            bool next : 1;
        } flags;

        FlexHeader flexHeader;

        std::vector<uint8_t> payload;

    private:
        static Message* createInstance(bool response, opcode_t opcode);
    };
}
