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

    class LIBGREENSTACK_PUBLIC_API ListBucketsRequest : public Greenstack::Request {
    public:
        ListBucketsRequest();

    protected:
        friend class Message;
    };

    class LIBGREENSTACK_PUBLIC_API ListBucketsResponse : public Greenstack::Response {
    public:
        ListBucketsResponse(const std::vector<std::string> &buckets);
        ListBucketsResponse(status_t status);
        ListBucketsResponse() : ListBucketsResponse(Greenstack::Status::Success) {}

        const std::vector<std::string> getBuckets() const;

    protected:
        friend class Message;
        virtual void validate() override;
    };
}
