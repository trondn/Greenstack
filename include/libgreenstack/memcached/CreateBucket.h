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

    namespace Bucket {
        typedef uint8_t bucket_type_t;
        const bucket_type_t Invalid = 0;
        const bucket_type_t Memcached = 1;
        const bucket_type_t Couchbase = 2;

        LIBGREENSTACK_PUBLIC_API
        std::string to_string(bucket_type_t bucket);

        LIBGREENSTACK_PUBLIC_API
        bucket_type_t from_string(const std::string &str);
    }

    class LIBGREENSTACK_PUBLIC_API CreateBucketRequest : public Greenstack::Request {
    public:
        CreateBucketRequest(const std::string &name, const std::string &config, Bucket::bucket_type_t type);
        const std::string getName() const;
        const std::string getConfig() const;
        const Bucket::bucket_type_t getType() const;

    protected:
        CreateBucketRequest();
        friend class Message;
        virtual void validate() override;
    };

    class LIBGREENSTACK_PUBLIC_API CreateBucketResponse : public Greenstack::Response {
    public:
        CreateBucketResponse();
        CreateBucketResponse(status_t status);

    protected:
        friend class Message;
    };
}
