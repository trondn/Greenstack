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
    typedef uint16_t status_t;
    namespace Status {

        // Generic status codes
        const status_t Success = 0x0000;
        const status_t InvalidArguments = 0x0001;
        const status_t InternalError = 0x0002;
        const status_t AuthenticationError = 0x0003;
        const status_t AuthenticationStale = 0x0004;
        const status_t NotInitialized = 0x0005;
        const status_t InvalidState = 0x0006;
        const status_t NoAccess = 0x0007;
        const status_t NotFound = 0x0008;
        const status_t UnknownCommand = 0x0009;
        const status_t UserAgentBlacklisted = 0x000a;
        const status_t NotImplemented = 0x000b;

        // Memcached status codes

        LIBGREENSTACK_PUBLIC_API
        std::string to_string(status_t status);
    }
}
