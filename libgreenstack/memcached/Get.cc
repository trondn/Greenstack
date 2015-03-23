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



#include "config.h"

#include <strings.h>
#include <map>

#include <libgreenstack/memcached/Get.h>
#include <flatbuffers/flatbuffers.h>
#include <Greenstack/payload/GetRequest_generated.h>
#include <Greenstack/payload/GetResponse_generated.h>


Greenstack::GetRequest::GetRequest(const std::string &id) : Request(Opcodes::Get) {
    if (id.empty()) {
        throw std::runtime_error("Document id can't be empty");
    }
    flatbuffers::FlatBufferBuilder fbb;
    auto _id = fbb.CreateString(id);

    Greenstack::Payload::GetRequestBuilder builder(fbb);
    builder.add_Id(_id);

    auto stuff = builder.Finish();
    Greenstack::Payload::FinishGetRequestBuffer(fbb, stuff);
    payload.resize(fbb.GetSize());
    memcpy(payload.data(), fbb.GetBufferPointer(), fbb.GetSize());

}

Greenstack::GetRequest::GetRequest() : Request(Opcodes::Get) {

}

const std::string Greenstack::GetRequest::getId() const {
    auto request = Greenstack::Payload::GetGetRequest(payload.data());
    std::string ret(request->Id()->c_str());
    return ret;
}

void Greenstack::GetRequest::validate() {
    Message::validate();

    using namespace Greenstack::Payload;
    using namespace flatbuffers;
    Verifier verifier(payload.data(), payload.size());

    if (!VerifyGetRequestBuffer(verifier)) {
        throw std::runtime_error("Incorrect payload for GetRequest");
    }

    if (!getFlexHeader().haveVbucketId()) {
        throw std::runtime_error("GetRequest needs vbucket id");
    }
}

Greenstack::GetResponse::GetResponse() : Response(Greenstack::Opcodes::Get, Greenstack::Status::Success) {

}

Greenstack::GetResponse::GetResponse(status_t status) : Response(Greenstack::Opcodes::Get, status) {

}


static Greenstack::Payload::compression_t convertCompression(Greenstack::compression_t compression) {
    switch (compression) {
        case Greenstack::Compression::None:
            return Greenstack::Payload::compression_t_None;
        case Greenstack::Compression::Snappy:
            return Greenstack::Payload::compression_t_Snappy;
        case Greenstack::Compression::BZip:
            return Greenstack::Payload::compression_t_Bzip;
        case Greenstack::Compression::GZip:
            return Greenstack::Payload::compression_t_Gzip;
        default:
            throw std::runtime_error(Greenstack::Compression::to_string(compression));
    }
}

static Greenstack::Payload::datatype_t convertDatatype(Greenstack::datatype_t datatype) {
    switch (datatype) {
        case Greenstack::Datatype::Raw:
            return Greenstack::Payload::datatype_t_Raw;
        case Greenstack::Datatype::Json:
            return Greenstack::Payload::datatype_t_Json;
        default:
            throw std::runtime_error(Greenstack::Datatype::to_string(datatype));
    }
}

void Greenstack::GetResponse::assemble() {
    flatbuffers::FlatBufferBuilder fbb;

    if (documentInfo.get() == 0 || value.get() == 0) {
        throw std::runtime_error("Document Info and Payload muust be set for GetRequest");
    }

    auto docid = fbb.CreateString(documentInfo->getId());
    auto exp = fbb.CreateString(documentInfo->getExpiration());

    const uint8_t *ptr = value->getData();
    auto docValue = fbb.CreateVector(ptr, value->getSize());

    Greenstack::Payload::compression_t compr = convertCompression(documentInfo->getCompression());
    Greenstack::Payload::datatype_t datatype = convertDatatype(documentInfo->getDatatype());

    Greenstack::Payload::DocumentInfoBuilder docinfoBuilder(fbb);
    docinfoBuilder.add_Expiration(exp);
    docinfoBuilder.add_Flags(documentInfo->getFlags());
    docinfoBuilder.add_Id(docid);
    docinfoBuilder.add_Datatype(datatype);
    docinfoBuilder.add_Compression(compr);
    docinfoBuilder.add_Cas(documentInfo->getCas());
    auto docinfoOffset = docinfoBuilder.Finish();

    Greenstack::Payload::DocumentBuilder docBuilder(fbb);
    docBuilder.add_Info(docinfoOffset);
    docBuilder.add_Value(docValue);
    auto docBuilderOffset = docBuilder.Finish();

    Greenstack::Payload::GetResponseBuilder getResponseBuilder(fbb);
    getResponseBuilder.add_Document(docBuilderOffset);

    auto stuff = getResponseBuilder.Finish();
    Greenstack::Payload::FinishGetResponseBuffer(fbb, stuff);
    payload.resize(fbb.GetSize());
    memcpy(payload.data(), fbb.GetBufferPointer(), fbb.GetSize());
}

void Greenstack::GetResponse::disassemble() {
    using namespace Greenstack::Payload;
    auto response = GetGetResponse(payload.data());

    auto document = response->Document();
    auto val = document->Value();
    // @todo verify that this is safe!!!!
    value = std::make_shared<FixedByteBuffer>(const_cast<uint8_t *>(val->Data()), val->Length());

    // Decode the DocumentInfo
    auto info = document->Info();
    documentInfo = std::make_shared<Greenstack::DocumentInfo>();

    documentInfo->setId(info->Id()->c_str());
    documentInfo->setFlags(info->Flags());
    documentInfo->setExpiration(info->Expiration()->c_str());
    documentInfo->setCas(info->Cas());
    documentInfo->setCompression(convertCompression(info->Compression()));
    documentInfo->setDatatype(convertDatatype(info->Datatype()));
}

void Greenstack::GetResponse::validate() {
    Message::validate();
    if (getStatus() == Status::Success) {
        using namespace Greenstack::Payload;
        using namespace flatbuffers;
        Verifier verifier(payload.data(), payload.size());

        if (!VerifyGetResponseBuffer(verifier)) {
            throw std::runtime_error("Incorrect payload for GetResponse");
        }
    } else {
        if (!payload.empty()) {
            throw std::runtime_error("Incorrect payload for GetResponse");
        }
    }
}