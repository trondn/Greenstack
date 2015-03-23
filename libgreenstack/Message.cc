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

#include <libgreenstack/Message.h>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <libgreenstack/Response.h>
#include <libgreenstack/Request.h>
#include "Writer.h"
#include "Reader.h"

Greenstack::Message::Message(bool response) : opaque(std::numeric_limits<uint32_t>::max()),
                                              opcode(std::numeric_limits<uint16_t>::max()) {
    memset(&flags, 0, sizeof(flags));
    flags.response = response;
}

Greenstack::Message::~Message() {

}

void Greenstack::Message::setOpaque(uint32_t value) {
    opaque = value;
}

uint32_t Greenstack::Message::getOpaque(void) const {
    return opaque;
}

void Greenstack::Message::setOpcode(uint16_t value) {
    opcode = value;
}

uint16_t Greenstack::Message::getOpcode(void) const {
    return opcode;
}

void Greenstack::Message::setFenceBit(bool enable) {
    flags.fence = enable;
}

bool Greenstack::Message::isFenceBitSet(void) const {
    return flags.fence;
}

void Greenstack::Message::setMoreBit(bool enable) {
    flags.more = enable;
}

bool Greenstack::Message::isMoreBitSet(void) const {
    return flags.more;
}

void Greenstack::Message::setQuietBit(bool enable) {
    flags.quiet = enable;
}

bool Greenstack::Message::isQuietBitSet(void) const {
    return flags.quiet;
}

Greenstack::FlexHeader &Greenstack::Message::getFlexHeader(void) {
    return flexHeader;
}

const Greenstack::FlexHeader &Greenstack::Message::getFlexHeader(void) const {
    return flexHeader;
}

void Greenstack::Message::setPayload(std::vector<uint8_t> &data) {
    payload.assign(data.begin(), data.end());
}

/**
* Encode the message into the byte array. The format of the packet is
*
*        0         1         2         3
*   +---------+---------+---------+---------+
*   |  Opaque                               |
*   +---------+---------+---------+---------+
*   |  Opcode           |  Flags  |
*   +---------+---------+---------+
*   7 bytes
*
*   If the response bit in the flag section is set then the
*   following layout follows
*
*        0         1
*   +---------+---------+
*   |  Status           |
*   +---------+---------+
*
*   If the flex header bit in the flag section is set the following
*   layout follows:
*
*        0         1         2         3
*   +---------+---------+---------+---------+
*   |  Flex header length                   |
*   +---------+---------+---------+---------+
*   |  N bytes with flex header data        |
*   +---------+---------+---------+---------+
*   4 + n bytes
*
*   Finally the actual data payload for the command follows
*/
size_t Greenstack::Message::encode(std::vector<uint8_t> &vector, size_t offset) const {
    ByteArrayWriter writer(vector, offset);

    writer.write(opaque);
    writer.write(opcode);

    Flags theFlags = flags;
    theFlags.flexHeaderPresent = !flexHeader.isEmpty();

    // This is a hack to convert my bitfield to a byte
    writer.write(reinterpret_cast<uint8_t *>(&theFlags), 1);

    // move offset past the fixed size header:
    offset += 7;

    if (flags.response) {
        const Response *r = dynamic_cast<const Response *>(this);
        assert(r);
        writer.write(r->getStatus());
        offset += 2;
    }

    if (theFlags.flexHeaderPresent) {
        // the header should be written after the length field
        offset += 4;
        ByteArrayWriter out(vector, offset);
        size_t nbytes = flexHeader.encode(out);
        writer.write(static_cast<uint32_t>(nbytes));
        writer.skip(nbytes);
    }

    if (payload.size() > 0) {
        writer.write(payload.data(), payload.size());
    }

    return writer.getOffset();
}

Greenstack::Message *Greenstack::Message::create(const std::vector<uint8_t> &vector, size_t offset, size_t end) {
    ByteArrayReader reader(vector, offset, end);
    Message *ret;

    uint32_t opaque;
    uint16_t opcode;
    struct Flags flags;

    reader.read(opaque);
    reader.read(opcode);
    reader.read(reinterpret_cast<uint8_t*>(&flags), 1);

    if (flags.response) {
        ret = new Response();
    } else {
        ret = new Request();
    }

    ret->opaque = opaque;
    ret->opcode = opcode;
    ret->flags = flags;
    offset += 7; // skip header

    if (flags.response) {
        Response *r = dynamic_cast<Response *>(ret);
        uint16_t status;
        reader.read(status);
        r->setStatus(status);
        offset += 2; // skip status
    }

    if (flags.flexHeaderPresent) {
        uint32_t nbytes;
        reader.read(nbytes);
        offset += 4; // skip length
        ByteArrayReader in(vector, offset, offset + nbytes);
        auto flex = FlexHeader::create(in);
        ret->flexHeader = flex;
        reader.skip(nbytes);
    }

    // Set the content
    ret->payload.resize(reader.getReminder());
    reader.read(ret->payload.data(), reader.getReminder());

    return ret;
}
