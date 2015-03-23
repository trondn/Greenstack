/* -*- Mode: C; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
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

#include <map>
#include <strings.h>
#include <libgreenstack/memcached/CreateBucket.h>
#include <flatbuffers/flatbuffers.h>
#include <Greenstack/payload/CreateBucketRequest_generated.h>


const std::map<Greenstack::Bucket::bucket_type_t, std::string> mappings = {
        {Greenstack::Bucket::Invalid,   "Invalid"},
        {Greenstack::Bucket::Memcached, "Memcached"},
        {Greenstack::Bucket::Couchbase, "Couchbase"}
};

std::string Greenstack::Bucket::to_string(Greenstack::Bucket::bucket_type_t bucket) {
    const auto iter = mappings.find(bucket);
    if (iter == mappings.end()) {
        return std::to_string(bucket);
    } else {
        return iter->second;
    }
}

Greenstack::Bucket::bucket_type_t Greenstack::Bucket::from_string(const std::string &val) {
    for (auto iter : mappings) {
        if (strcasecmp(val.c_str(), iter.second.c_str()) == 0) {
            return iter.first;
        }
    }

    std::string msg = "Unknown bucket type [";
    msg.append(val);
    msg.append("]");
    throw std::runtime_error(msg);
}


Greenstack::Bucket::bucket_type_t const Greenstack::CreateBucketRequest::getType() const {
    auto request = Greenstack::Payload::GetCreateBucketRequest(payload.data());
    auto type = request->Type();
    switch (type) {
        case Greenstack::Payload::bucket_type_t_Memcached:
            return Greenstack::Bucket::Memcached;
        case Greenstack::Payload::bucket_type_t_Couchbase:
            return Greenstack::Bucket::Couchbase;
        default: {
            std::string msg = "Unsupported bucket type";
            throw std::runtime_error(msg);
        }
    }
}

const std::string Greenstack::CreateBucketRequest::getName() const {
    auto request = Greenstack::Payload::GetCreateBucketRequest(payload.data());
    std::string ret(request->Name()->c_str());
    return ret;
}

const std::string Greenstack::CreateBucketRequest::getConfig() const {
    auto request = Greenstack::Payload::GetCreateBucketRequest(payload.data());
    std::string ret(request->Config()->c_str());
    return ret;
}

Greenstack::CreateBucketRequest::CreateBucketRequest(const std::string &name, const std::string &config,
                                                     Greenstack::Bucket::bucket_type_t type) : Request(
        Opcodes::CreateBucket) {
    if (name.empty()) {
        throw std::runtime_error("Bucket name can't be empty");
    }
    flatbuffers::FlatBufferBuilder fbb;
    auto bucketName = fbb.CreateString(name);
    auto bucketConfig = fbb.CreateString(config);

    Greenstack::Payload::CreateBucketRequestBuilder builder(fbb);
    builder.add_Name(bucketName);
    builder.add_Config(bucketConfig);
    switch (type) {
        case Greenstack::Bucket::Memcached:
            builder.add_Type(Greenstack::Payload::bucket_type_t_Memcached);
            break;
        case Greenstack::Bucket::Couchbase:
            builder.add_Type(Greenstack::Payload::bucket_type_t_Couchbase);
            break;
        default:
            std::string msg = "Unsupported bucket type";
            throw std::runtime_error(msg);
    }

    auto stuff = builder.Finish();
    Greenstack::Payload::FinishCreateBucketRequestBuffer(fbb, stuff);
    payload.resize(fbb.GetSize());
    memcpy(payload.data(), fbb.GetBufferPointer(), fbb.GetSize());
}

Greenstack::CreateBucketRequest::CreateBucketRequest() : Request(Opcodes::CreateBucket) {

}

void Greenstack::CreateBucketRequest::validate() {
    Message::validate();
    using namespace Greenstack::Payload;
    using namespace flatbuffers;
    Verifier verifier(payload.data(), payload.size());

    if (!VerifyCreateBucketRequestBuffer(verifier)) {
        throw std::runtime_error("Incorrect payload for CreateBucketRequest");
    }
}

Greenstack::CreateBucketResponse::CreateBucketResponse() : Response(Opcodes::CreateBucket, Status::Success) {

}

Greenstack::CreateBucketResponse::CreateBucketResponse(Greenstack::status_t status) : Response(Opcodes::CreateBucket, status) {

}
