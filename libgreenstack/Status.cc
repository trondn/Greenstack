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
#include <libgreenstack/Status.h>
#include <map>

const std::map<Greenstack::status_t, std::string> mappings = {
        {Greenstack::Status::Success,              "Success"},
        {Greenstack::Status::InvalidArguments,     "Invalid arguments"},
        {Greenstack::Status::InternalError,        "Internal error"},
        {Greenstack::Status::AuthenticationError,  "Authentication error"},
        {Greenstack::Status::AuthenticationStale,  "Authentication stale"},
        {Greenstack::Status::NotInitialized,       "Not initialized"},
        {Greenstack::Status::InvalidState,         "Invalid state"},
        {Greenstack::Status::NoAccess,             "No access"},
        {Greenstack::Status::NotFound,             "Not found"},
        {Greenstack::Status::UnknownCommand,       "Unknown command"},
        {Greenstack::Status::UserAgentBlacklisted, "User agent blacklisted"},
        {Greenstack::Status::NotImplemented,       "Not implemented"}
};


std::string Greenstack::Status::to_string(status_t status) {
    const auto iter = mappings.find(status);
    if (iter == mappings.cend()) {
        return std::to_string(status);
    } else {
        return iter->second;
    }
}
