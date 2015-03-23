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

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <libgreenstack/visibility.h>

namespace Greenstack {
    class Writer;
    class ByteArrayReader;
    class Request;
    class Response;
    class Message;

    class LIBGREENSTACK_PUBLIC_API FlexHeader {
    public:
        static FlexHeader create(ByteArrayReader &reader);

        bool isEmpty() const {
            return header.empty();
        }

        void setLaneId(const std::string &lane);
        bool haveLaneId(void) const;
        std::string getLaneId(void) const;

        void setCompression(const std::string &compression);
        bool haveCompression(void) const;
        std::string getCompression(void) const;

        void setDatatype(const std::string &datatype);
        bool haveDatatype(void) const;
        std::string getDatatype(void) const;

        void setCAS(const std::vector<uint8_t> &cas);
        bool haveCAS(void) const;
        std::vector<uint8_t> getCAS(void) const;

        void setTXID(const std::string &txid);
        bool haveTXID(void) const;
        std::string getTXID(void) const;

        void setPriority(uint8_t priority);
        bool havePriority(void) const;
        uint8_t getPriority(void) const;

        void setDcpId(const std::string &dcpid);
        bool haveDcpId(void) const;
        std::string getDcpId(void) const;

        void setVbucketId(uint16_t vbid);
        bool haveVbucketId(void) const;
        uint16_t getVbucketId(void) const;

        void setHash(uint32_t hash);
        bool haveHash(void) const;
        uint32_t getHash(void) const;

        void setTimeout(uint32_t value);
        bool haveTimeout(void) const;
        uint32_t getTimeout(void) const;

        void setCommandTimings(const std::string &value);
        bool haveCommandTimings(void) const;
        std::string getCommandTimings(void) const;

    protected:
        size_t encode(Writer &writer) const;

        friend class Request;
        friend class Response;
        friend class Message;

        void insertField(uint16_t opcode, const uint8_t *value, size_t length);

        std::map<uint16_t, std::vector<uint8_t> > header;
    };
}
