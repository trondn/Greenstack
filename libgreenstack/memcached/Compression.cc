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
#include "config.h"
#include <libgreenstack/memcached/Compression.h>
#include <map>
#include <stdexcept>
#include <strings.h>

const std::map<Greenstack::compression_t, std::string> mappings = {
        {Greenstack::Compression::None,   "None"},
        {Greenstack::Compression::Snappy, "Snappy"},
        {Greenstack::Compression::GZip,   "GZip"},
        {Greenstack::Compression::BZip,   "BZip"}
};


std::string Greenstack::Compression::to_string(Greenstack::compression_t compression) {
    const auto iter = mappings.find(compression);
    if (iter == mappings.end()) {
        return std::to_string(compression);
    } else {
        return iter->second;
    }
}

Greenstack::compression_t Greenstack::Compression::from_string(const std::string &str) {
    for (auto iter : mappings) {
        if (strcasecmp(str.c_str(), iter.second.c_str()) == 0) {
            return iter.first;
        }
    }

    std::string msg = "Unknown compression [";
    msg.append(str);
    msg.append("]");
    throw std::runtime_error(msg);
}
