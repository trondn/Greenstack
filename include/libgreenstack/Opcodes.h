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

#include <libgreenstack/visibility.h>

#include <cstdint>
#include <string>

namespace Greenstack {
    typedef uint16_t opcode_t;

    namespace Opcodes {
        // Generic packet opcodes (0x0000 - 0x03ff)
        const opcode_t Hello = 0x0001;
        const opcode_t SaslAuth = 0x0002;

        // Memcached specific opcodes (0x0000 - 0x03ff)
        const opcode_t SelectBucket = 0x0400;
        const opcode_t ListBuckets = 0x0401;
        const opcode_t CreateBucket = 0x0402;
        const opcode_t DeleteBucket = 0x0403;
        const opcode_t AssumeRole = 0x0404;
        const opcode_t Mutation = 0x0405;
        const opcode_t Get = 0x0406;

        LIBGREENSTACK_PUBLIC_API
        std::string to_string(opcode_t opcode);

        LIBGREENSTACK_PUBLIC_API
        opcode_t from_string(const std::string &str);
    }
}
