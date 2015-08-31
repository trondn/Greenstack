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
#include <libgreenstack/Request.h>
#include <libgreenstack/Response.h>
#include <string>

namespace Greenstack {
    class Message;

    class LIBGREENSTACK_PUBLIC_API SaslAuthRequest : public Greenstack::Request {
    public:
        SaslAuthRequest(const std::string &mechanism, const std::string &challenge);
        const std::string getMechanism() const;
        const std::string getChallenge() const;

    protected:
        SaslAuthRequest();
        friend class Message;
        virtual void validate() override;
    };

    class LIBGREENSTACK_PUBLIC_API SaslAuthResponse : public Greenstack::Response {
    public:
        SaslAuthResponse(uint16_t status);
        SaslAuthResponse(const std::string &challenge);
        const std::string getChallenge() const;

    protected:
        SaslAuthResponse();
        friend class Message;
        virtual void validate() override;
    };
}
