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
#include <libgreenstack/Greenstack.h>
#include <libgreenstack/MessageStream.h>
#include <cassert>

int main(void) {
#if 0
    Greenstack::MessageStream stream;
    Greenstack::Request request;
    Greenstack::Response response;

    stream.getLocal().send(&request);
    stream.getPeer().send(&response);

    Greenstack::Message *rq = stream.getPeer().recv();
    Greenstack::Message *rs = stream.getLocal().recv();

    assert(rq != NULL);
    assert(dynamic_cast<Greenstack::Request*>(rq) != NULL);
    assert(rs != NULL);
    assert(dynamic_cast<Greenstack::Response*>(rs) != NULL);

    delete rq;
    delete rs;
#endif
    return EXIT_SUCCESS;
}
