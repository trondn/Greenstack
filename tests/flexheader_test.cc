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
#include <libgreenstack/FlexHeader.h>
#include "../libgreenstack/Writer.h"
#include "../libgreenstack/Reader.h"
#include <iostream>

using namespace Greenstack;

class FlexHeaderTest : public FlexHeader {
public:
    void encode(ByteArrayWriter &writer) {
        FlexHeader::encode(writer);
    }
};

static bool lane_id_test(void) {
    FlexHeaderTest header;
    if (header.haveLaneId()) {
        std::cerr << "FAIL: Lane id should not be set by the default constructor" << std::endl;
        return false;
    }

    try {
        header.getLaneId();
        std::cerr << "FAIL: Lane id should not be set by the default constructor" << std::endl;
        return false;
    } catch (...) {}

    header.setLaneId("foo");
    try {
        header.setLaneId("foo");
        std::cerr << "FAIL: it is possible to set lane id multiple times" << std::endl;
        return false;
    } catch (...) {
        // empty
    }

    if (!header.haveLaneId()) {
        std::cerr << "FAIL: Lane ID should be set" << std::endl;
        return false;
    }

    try {
        std::string laneid = header.getLaneId();
        if (laneid != "foo") {
            std::cerr << "FAIL: expected Lane id to be foo, got \"" << laneid << "\"" << std::endl;
            return false;
        }
    } catch (...) {
        std::cerr << "FAIL: failed to get lane id" << std::endl;
    }

    std::vector<uint8_t> data;
    ByteArrayWriter writer(data);

    header.encode(writer);
    if (data.size() != 7) {
        std::cerr << "FAIL: encode of lane id is incorrect (length)" << std::endl;
        return false;
    }

    if (data[0] != 0x00 || data[1] != 0x00 || data[2] != 0x00 || data[3] != 0x03 || data[4] != 'f' || data[5] != 'o' || data[6] != 'o') {
        std::cerr << "FAIL: encode of lane id is incorrect (content)" << std::endl;
        return false;
    }
    return true;
}

static bool compression_test(void) {
    FlexHeaderTest header;
    if (header.haveCompression()) {
        std::cerr << "FAIL: Compression should not be set by the default constructor" << std::endl;
        return false;
    }

    try {
        header.getCompression();
        std::cerr << "FAIL: Compression should not be set by the default constructor" << std::endl;
        return false;
    } catch (...) {}

    header.setCompression("zip");
    try {
        header.setCompression("gzip");
        std::cerr << "FAIL: it is possible to set Compression multiple times" << std::endl;
        return false;
    } catch (...) {
        // empty
    }

    if (!header.haveCompression()) {
        std::cerr << "FAIL: Compression should be set" << std::endl;
        return false;
    }

    try {
        std::string compression = header.getCompression();
        if (compression != "zip") {
            std::cerr << "FAIL: expected compression to be zip, got \"" << compression << "\"" << std::endl;
            return false;
        }
    } catch (...) {
        std::cerr << "FAIL: failed to get compression" << std::endl;
    }

    std::vector<uint8_t> data;
    ByteArrayWriter writer(data);

    header.encode(writer);
    if (data.size() != 7) {
        std::cerr << "FAIL: encode of compression is incorrect (length)" << std::endl;
        return false;
    }

    if (data[0] != 0x00 || data[1] != 0x01 || data[2] != 0x00 || data[3] != 3 || data[4] != 'z' || data[5] != 'i' || data[6] != 'p') {
        std::cerr << "FAIL: encode of compression is incorrect (content)" << std::endl;
        return false;
    }
    return true;
}

static bool datatype_test(void) {
    FlexHeaderTest header;
    if (header.haveDatatype()) {
        std::cerr << "FAIL: Datatype should not be set by the default constructor" << std::endl;
        return false;
    }

    try {
        header.getDatatype();
        std::cerr << "FAIL: Datatype should not be set by the default constructor" << std::endl;
        return false;
    } catch (...) {}

    header.setDatatype("json");
    try {
        header.setDatatype("blob");
        std::cerr << "FAIL: it is possible to set datatype multiple times" << std::endl;
        return false;
    } catch (...) {
        // empty
    }

    if (!header.haveDatatype()) {
        std::cerr << "FAIL: Datatype should be set" << std::endl;
        return false;
    }

    try {
        std::string datatype = header.getDatatype();
        if (datatype != "json") {
            std::cerr << "FAIL: expected datatype to be json, got \"" << datatype << "\"" << std::endl;
            return false;
        }
    } catch (...) {
        std::cerr << "FAIL: failed to get datatype" << std::endl;
    }

    std::vector<uint8_t> data;
    ByteArrayWriter writer(data);

    header.encode(writer);
    if (data.size() != 8) {
        std::cerr << "FAIL: encode of datatype is incorrect (length)" << std::endl;
        return false;
    }

    if (data[0] != 0x00 || data[1] != 0x02 || data[2] != 0x00 || data[3] != 0x04 || data[4] != 'j' || data[5] != 's' || data[6] != 'o' || data[7] != 'n') {
        std::cerr << "FAIL: encode of datatype is incorrect (value)" << std::endl;
        return false;
    }
    return true;
}

static bool cas_test(void) {
    FlexHeaderTest header;
    if (header.haveCAS()) {
        std::cerr << "FAIL: CAS should not be set by the default constructor" << std::endl;
        return false;
    }

    try {
        header.getCAS();
        std::cerr << "FAIL: CAS should not be set by the default constructor" << std::endl;
        return false;
    } catch (...) {}

    std::vector<uint8_t> cas;
    try {
        header.setCAS(cas);
        std::cerr << "FAIL: CAS should not be 8 byte long" << std::endl;
        return false;
    } catch (...) {}

    cas.resize(8);

    header.setCAS(cas);
    try {
        header.setCAS(cas);
        std::cerr << "FAIL: it is possible to set cas multiple times" << std::endl;
        return false;
    } catch (...) {
        // empty
    }

    if (!header.haveCAS()) {
        std::cerr << "FAIL: CAS should be set" << std::endl;
        return false;
    }

    try {
        std::vector<uint8_t> val = header.getCAS();
        if (cas != val) {
            std::cerr << "FAIL: Received different CAS" << std::endl;
            return false;
        }
    } catch (...) {
        std::cerr << "FAIL: failed to get CAS" << std::endl;
    }

    std::vector<uint8_t> data;
    ByteArrayWriter writer(data);

    header.encode(writer);
    if (data.size() != 12) {
        std::cerr << "FAIL: encode of CAS is incorrect (length)" << std::endl;
        return false;
    }

    if (data[0] != 0x00 || data[1] != 0x03 || data[2] != 0x00 || data[3] != 0x08) {
        std::cerr << "FAIL: encode of CAS is incorrect (content)" << std::endl;
        return false;
    }

    auto iter = data.begin();
    iter += 4; // skip key and length

    if (!equal(iter, data.end(), cas.begin())) {
        std::cerr << "FAIL: encode of CAS is incorrect" << std::endl;
        return false;
    }

    return true;
}

static bool txid_test(void) {
    FlexHeaderTest header;
    if (header.haveTXID()) {
        std::cerr << "FAIL: txid should not be set by the default constructor" << std::endl;
        return false;
    }

    try {
        header.getTXID();
        std::cerr << "FAIL: txid should not be set by the default constructor" << std::endl;
        return false;
    } catch (...) {}

    header.setTXID("txid");
    try {
        header.setTXID("blob");
        std::cerr << "FAIL: it is possible to set txid multiple times" << std::endl;
        return false;
    } catch (...) {
        // empty
    }

    if (!header.haveTXID()) {
        std::cerr << "FAIL: txid should be set" << std::endl;
        return false;
    }

    try {
        std::string txid = header.getTXID();
        if (txid != "txid") {
            std::cerr << "FAIL: expected txid to be txid, got \"" << txid << "\"" << std::endl;
            return false;
        }
    } catch (...) {
        std::cerr << "FAIL: failed to get txid" << std::endl;
    }

    std::vector<uint8_t> data;
    ByteArrayWriter writer(data);

    header.encode(writer);
    if (data.size() != 8) {
        std::cerr << "FAIL: encode of txid is incorrect (length)" << std::endl;
        return false;
    }

    if (data[0] != 0x00 || data[1] != 0x04 || data[2] != 0x00 || data[3] != 0x04 || data[4] != 't' || data[5] != 'x' || data[6] != 'i' || data[7] != 'd') {
        std::cerr << "FAIL: encode of txid is incorrect (value)" << std::endl;
        return false;
    }
    return true;
}

static bool dcpid_test(void) {
    FlexHeaderTest header;
    if (header.haveDcpId()) {
        std::cerr << "FAIL: dcpid should not be set by the default constructor" << std::endl;
        return false;
    }

    try {
        header.getDcpId();
        std::cerr << "FAIL: dcpid should not be set by the default constructor" << std::endl;
        return false;
    } catch (...) {}

    header.setDcpId("DCP");
    try {
        header.setDcpId("blob");
        std::cerr << "FAIL: it is possible to set dcpid multiple times" << std::endl;
        return false;
    } catch (...) {
        // empty
    }

    if (!header.haveDcpId()) {
        std::cerr << "FAIL: dcpid should be set" << std::endl;
        return false;
    }

    try {
        std::string dcpid = header.getDcpId();
        if (dcpid != "DCP") {
            std::cerr << "FAIL: expected dcpid to be DCP, got \"" << dcpid << "\"" << std::endl;
            return false;
        }
    } catch (...) {
        std::cerr << "FAIL: failed to get dcpid" << std::endl;
    }

    std::vector<uint8_t> data;
    ByteArrayWriter writer(data);

    header.encode(writer);
    if (data.size() != 7) {
        std::cerr << "FAIL: encode of dcpid is incorrect (length)" << std::endl;
        return false;
    }

    if (data[0] != 0x00 || data[1] != 0x06 || data[2] != 0x00 || data[3] != 0x03 || data[4] != 'D' || data[5] != 'C' || data[6] != 'P') {
        std::cerr << "FAIL: encode of dcpid is incorrect (value)" << std::endl;
        return false;
    }
    return true;
}

static bool command_timings_test(void) {
    FlexHeaderTest header;
    if (header.haveCommandTimings()) {
        std::cerr << "FAIL: command timings should not be set by the default constructor" << std::endl;
        return false;
    }

    try {
        header.getCommandTimings();
        std::cerr << "FAIL: command timings should not be set by the default constructor" << std::endl;
        return false;
    } catch (...) {}

    header.setCommandTimings("foo");
    try {
        header.setCommandTimings("blob");
        std::cerr << "FAIL: it is possible to set command timings multiple times" << std::endl;
        return false;
    } catch (...) {
        // empty
    }

    if (!header.haveCommandTimings()) {
        std::cerr << "FAIL: command timings should be set" << std::endl;
        return false;
    }

    try {
        std::string cmd = header.getCommandTimings();
        if (cmd != "foo") {
            std::cerr << "FAIL: expected command timings to be foo, got \"" << cmd << "\"" << std::endl;
            return false;
        }
    } catch (...) {
        std::cerr << "FAIL: failed to get command timings" << std::endl;
    }

    std::vector<uint8_t> data;
    ByteArrayWriter writer(data);

    header.encode(writer);
    if (data.size() != 7) {
        std::cerr << "FAIL: encode of command timings is incorrect (length)" << std::endl;
        return false;
    }

    if (data[0] != 0x00 || data[1] != 0x0a || data[2] != 0x00 || data[3] != 0x03 || data[4] != 'f' || data[5] != 'o' || data[6] != 'o') {
        std::cerr << "FAIL: encode of command timings is incorrect (value)" << std::endl;
        return false;
    }
    return true;
}

static bool priority_test(void) {
    FlexHeaderTest header;
    if (header.havePriority()) {
        std::cerr << "FAIL: priority should not be set by the default constructor" << std::endl;
        return false;
    }

    try {
        header.getPriority();
        std::cerr << "FAIL: priority should not be set by the default constructor" << std::endl;
        return false;
    } catch (...) {}

    header.setPriority(0);
    try {
        header.setPriority(1);
        std::cerr << "FAIL: it is possible to set priority multiple times" << std::endl;
        return false;
    } catch (...) {
        // empty
    }

    if (!header.havePriority()) {
        std::cerr << "FAIL: priority should be set" << std::endl;
        return false;
    }

    try {
        auto val = header.getPriority();
        if (val != 0) {
            std::cerr << "FAIL: expected priority to be 0, got \"" << val << "\"" << std::endl;
            return false;
        }
    } catch (...) {
        std::cerr << "FAIL: failed to get priority" << std::endl;
    }

    std::vector<uint8_t> data;
    ByteArrayWriter writer(data);

    header.encode(writer);
    if (data.size() != 5) {
        std::cerr << "FAIL: encode of priority is incorrect (length)" << std::endl;
        return false;
    }

    if (data[0] != 0x00 || data[1] != 0x05 || data[2] != 0x00 || data[3] != 0x01 || data[4] != 0) {
        std::cerr << "FAIL: encode of priority is incorrect (value)" << std::endl;
        return false;
    }
    return true;
}

static bool vbucketid_test(void) {
    FlexHeaderTest header;
    if (header.haveVbucketId()) {
        std::cerr << "FAIL: vbucketid should not be set by the default constructor" << std::endl;
        return false;
    }

    try {
        header.getVbucketId();
        std::cerr << "FAIL: vbucketid should not be set by the default constructor" << std::endl;
        return false;
    } catch (...) {}

    header.setVbucketId(0xaa);
    try {
        header.setVbucketId(0xbb);
        std::cerr << "FAIL: it is possible to set vbucketid multiple times" << std::endl;
        return false;
    } catch (...) {
        // empty
    }

    if (!header.haveVbucketId()) {
        std::cerr << "FAIL: vbucketid should be set" << std::endl;
        return false;
    }

    try {
        auto val = header.getVbucketId();
        if (val != 0xaa) {
            std::cerr << "FAIL: expected vbucketid to be aa, got \"" << val << "\"" << std::endl;
            return false;
        }
    } catch (...) {
        std::cerr << "FAIL: failed to get vbucketid" << std::endl;
    }

    std::vector<uint8_t> data;
    ByteArrayWriter writer(data);

    header.encode(writer);
    if (data.size() != 6) {
        std::cerr << "FAIL: encode of vbucketid is incorrect (length)" << std::endl;
        return false;
    }

    if (data[0] != 0x00 || data[1] != 0x07 || data[2] != 0x00 || data[3] != 0x02 || data[4] != 0 || data[5] != 0xaa) {
        std::cerr << "FAIL: encode of vbucketid is incorrect (value)" << std::endl;
        return false;
    }
    return true;
}

static bool hash_test(void) {
    FlexHeaderTest header;
    if (header.haveHash()) {
        std::cerr << "FAIL: should not be set by the default constructor" << std::endl;
        return false;
    }

    try {
        header.getHash();
        std::cerr << "FAIL: should not be set by the default constructor" << std::endl;
        return false;
    } catch (...) {}

    header.setHash(0xaa);
    try {
        header.setHash(0xbb);
        std::cerr << "FAIL: it is possible to set hash multiple times" << std::endl;
        return false;
    } catch (...) {
        // empty
    }

    if (!header.haveHash()) {
        std::cerr << "FAIL: should be set" << std::endl;
        return false;
    }

    try {
        auto val = header.getHash();
        if (val != 0xaa) {
            std::cerr << "FAIL: expected aa, got \"" << val << "\"" << std::endl;
            return false;
        }
    } catch (...) {
        std::cerr << "FAIL: failed to get hash" << std::endl;
    }

    std::vector<uint8_t> data;
    ByteArrayWriter writer(data);

    header.encode(writer);
    if (data.size() != 8) {
        std::cerr << "FAIL: encode is incorrect (length)" << std::endl;
        return false;
    }

    if (data[0] != 0x00 || data[1] != 0x08 || data[2] != 0x00 || data[3] != 0x04 || data[4] != 0 || data[5] != 0 || data[6] != 0 || data[7] != 0xaa) {
        std::cerr << "FAIL: encode is incorrect (value)" << std::endl;
        return false;
    }
    return true;
}

static bool command_timeout_test(void) {
    FlexHeaderTest header;
    if (header.haveTimeout()) {
        std::cerr << "FAIL: should not be set by the default constructor" << std::endl;
        return false;
    }

    try {
        header.getTimeout();
        std::cerr << "FAIL: should not be set by the default constructor" << std::endl;
        return false;
    } catch (...) {}

    header.setTimeout(0xaa);
    try {
        header.setTimeout(0xbb);
        std::cerr << "FAIL: it is possible to set timeout multiple times" << std::endl;
        return false;
    } catch (...) {
        // empty
    }

    if (!header.haveTimeout()) {
        std::cerr << "FAIL: should be set" << std::endl;
        return false;
    }

    try {
        auto val = header.getTimeout();
        if (val != 0xaa) {
            std::cerr << "FAIL: expected aa, got \"" << val << "\"" << std::endl;
            return false;
        }
    } catch (...) {
        std::cerr << "FAIL: failed to get timeout" << std::endl;
    }

    std::vector<uint8_t> data;
    ByteArrayWriter writer(data);

    header.encode(writer);
    if (data.size() != 8) {
        std::cerr << "FAIL: encode is incorrect (length)" << std::endl;
        return false;
    }

    if (data[0] != 0x00 || data[1] != 0x09 || data[2] != 0x00 || data[3] != 0x04 || data[4] != 0 || data[5] != 0 || data[6] != 0 || data[7] != 0xaa) {
        std::cerr << "FAIL: encode is incorrect" << std::endl;
        return false;
    }
    return true;
}

bool full_mix_test(void) {
    FlexHeaderTest header;

    header.setLaneId("foo");
    header.setCompression("gz");
    header.setDatatype("json");
    std::vector<uint8_t> cas;
    cas.resize(8);
    header.setCAS(cas);
    header.setTXID("txid");
    header.setDcpId("dcp channel");
    header.setPriority(1);
    header.setVbucketId(32);
    header.setHash(0xdeadbeef);
    header.setTimeout(100);
    header.setCommandTimings("get 32ms");

    std::vector<uint8_t> data;
    ByteArrayWriter writer(data);

    header.encode(writer);
    if (data.size() != 95) {
        std::cerr << "FAIL: Expected size of 95, got " << data.size() << std::endl;
        return false;
    }

    ByteArrayReader reader(data);
    try {
        FlexHeader decoded = FlexHeader::create(reader);
        if (decoded.getLaneId() != "foo") {
            std::cerr << "FAIL: Expected decoded lane to be foo" << std::endl;
            return false;
        }
        if (decoded.getCompression() != "gz") {
            std::cerr << "FAIL: Expected compression to be gz" << std::endl;
            return false;
        }
        if (decoded.getDatatype() != "json") {
            std::cerr << "FAIL: Expected datatype to be json" << std::endl;
            return false;
        }
        if (decoded.getTXID() != "txid") {
            std::cerr << "FAIL: Expected txid to be txid" << std::endl;
            return false;
        }
        if (decoded.getDcpId() != "dcp channel") {
            std::cerr << "FAIL: Expected dcpid to be dcp channel" << std::endl;
            return false;
        }
        if (decoded.getPriority() != 1) {
            std::cerr << "FAIL: Expected priority to be 1" << std::endl;
            return false;
        }
        if (decoded.getVbucketId() != 32) {
            std::cerr << "FAIL: Expected vbucket id to be 32" << std::endl;
            return false;
        }
        if (decoded.getHash() != 0xdeadbeef) {
            std::cerr << "FAIL: Expected hash id to be 0xdeadbeef" << std::endl;
            return false;
        }
        if (decoded.getTimeout() != 100) {
            std::cerr << "FAIL: Expected timeout to be 100" << std::endl;
            return false;
        }
        if (decoded.getCommandTimings() != "get 32ms") {
            std::cerr << "FAIL: Expected command timings to be get 32ms" << std::endl;
            return false;
        }
    } catch (...) {
        std::cerr << "FAIL: Failed to decode encoded buffer" << std::endl;
    }

    return true;
}

typedef bool (*testfunc)(void);

int main(void) {
    std::map<std::string, testfunc> tests;
    int failed(0);

    tests["lane id"] = lane_id_test;
    tests["compression"] = compression_test;
    tests["datatype"] = datatype_test;
    tests["cas"] = cas_test;
    tests["txid"] = txid_test;
    tests["dcp id"] = dcpid_test;
    tests["priority"] = priority_test;
    tests["vbucket id"] = vbucketid_test;
    tests["hash"] = hash_test;
    tests["command timeout"] = command_timeout_test;
    tests["command timings"] = command_timings_test;
    tests["full mix"] = full_mix_test;

    for (auto iter = tests.begin(); iter != tests.end(); ++iter) {
        std::cout << iter->first << "... ";
        std::cout.flush();
        if (iter->second()) {
            std::cout << "ok" << std::endl;
        } else {
            ++failed;
            // error should already be printed
        }
    }

    if (failed) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}
