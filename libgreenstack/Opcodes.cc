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
#include <libgreenstack/Opcodes.h>

#include <stdexcept>
#include <strings.h>
#include <map>

const std::map<Greenstack::opcode_t, std::string> mappings = {
        {Greenstack::Opcodes::Hello,        "Hello"},
        {Greenstack::Opcodes::SaslAuth,     "SaslAuth"},
        {Greenstack::Opcodes::SelectBucket, "SelectBucket"},
        {Greenstack::Opcodes::ListBuckets,  "ListBuckets"},
        {Greenstack::Opcodes::CreateBucket, "CreateBucket"},
        {Greenstack::Opcodes::DeleteBucket, "DeleteBucket"},
        {Greenstack::Opcodes::AssumeRole,   "AssumeRole"},
        {Greenstack::Opcodes::Mutation,     "Mutation"},
        {Greenstack::Opcodes::Get,          "Get"}
};

std::string Greenstack::Opcodes::to_string(Greenstack::opcode_t opcode) {
    const auto iter = mappings.find(opcode);
    if (iter == mappings.end()) {
        return std::to_string(opcode);
    } else {
        return iter->second;
    }
}

Greenstack::opcode_t Greenstack::Opcodes::from_string(const std::string &val) {
    for (auto iter : mappings) {
        if (strcasecmp(val.c_str(), iter.second.c_str()) == 0) {
            return iter.first;
        }
    }

    std::string msg = "Unknown command [";
    msg.append(val);
    msg.append("]");
    throw std::runtime_error(msg);
}
