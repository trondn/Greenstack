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
#pragma once

#ifdef greenstack_EXPORTS

#if defined (__SUNPRO_C) && (__SUNPRO_C >= 0x550)
#define LIBGREENSTACK_PUBLIC_API __global
#elif defined __GNUC__
#define LIBGREENSTACK_PUBLIC_API __attribute__ ((visibility("default")))
#elif defined(_MSC_VER)
// We're currently building a static archive on Windows due
// to all of the warnings from exporting STL members from the
// dll.. I guess I need to resolve that in some way..
//__declspec(dllexport)
#define LIBGREENSTACK_PUBLIC_API
#else
/* unknown compiler */
#define LIBGREENSTACK_PUBLIC_API
#endif

#else

#if defined(_MSC_VER)
#define LIBGREENSTACK_PUBLIC_API
// __declspec(dllimport)
#else
#define LIBGREENSTACK_PUBLIC_API
#endif

#endif
