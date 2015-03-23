include_directories(AFTER ${PROJECT_BINARY_DIR}/payload)
include_directories(AFTER ${FLATBUFFERS_INCLUDE_DIR})

add_library(greenstack STATIC
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Buffer.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/FlexHeader.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Frame.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Greenstack.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Message.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Opcodes.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Reader.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Request.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Response.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Status.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Writer.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/core/Hello.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/core/SaslAuth.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/memcached/AssumeRole.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/memcached/Cas.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/memcached/Compression.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/memcached/CreateBucket.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/memcached/Datatype.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/memcached/DeleteBucket.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/memcached/Document.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/memcached/DocumentInfo.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/memcached/Get.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/memcached/ListBuckets.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/memcached/Mutation.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/memcached/SelectBucket.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/visibility.h
            ${Greenstack_SOURCE_DIR}/payload/AssumeRoleRequest.fbs
            ${Greenstack_SOURCE_DIR}/payload/CreateBucketRequest.fbs
            ${Greenstack_SOURCE_DIR}/payload/DeleteBucketRequest.fbs
            ${Greenstack_SOURCE_DIR}/payload/Document.fbs
            ${Greenstack_SOURCE_DIR}/payload/HelloRequest.fbs
            ${Greenstack_SOURCE_DIR}/payload/HelloResponse.fbs
            ${Greenstack_SOURCE_DIR}/payload/GetRequest.fbs
            ${Greenstack_SOURCE_DIR}/payload/GetResponse.fbs
            ${Greenstack_SOURCE_DIR}/payload/ListBucketsResponse.fbs
            ${Greenstack_SOURCE_DIR}/payload/MutationRequest.fbs
            ${Greenstack_SOURCE_DIR}/payload/MutationResponse.fbs
            ${Greenstack_SOURCE_DIR}/payload/SaslAuthRequest.fbs
            ${Greenstack_SOURCE_DIR}/payload/SaslAuthResponse.fbs
            ${Greenstack_SOURCE_DIR}/payload/SelectBucketRequest.fbs

            core/Hello.cc
            core/SaslAuth.cc

            memcached/AssumeRole.cc
            memcached/Cas.cc
            memcached/Compression.cc
            memcached/CreateBucket.cc
            memcached/Datatype.cc
            memcached/DeleteBucket.cc
            memcached/DocumentInfo.cc
            memcached/Get.cc
            memcached/ListBuckets.cc
            memcached/Mutation.cc
            memcached/SelectBucket.cc

            FlexHeader.cc
            Frame.cc
            Message.cc
            Opcodes.cc
            Request.cc
            Response.cc
            Status.cc
            )

add_dependencies(greenstack generate_flatbuffer_headers)

if (WIN32)
   target_link_libraries(greenstack greenstack_platform ws2_32)
else (WIN32)
   target_link_libraries(greenstack greenstack_platform)
endif (WIN32)

install(TARGETS greenstack
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
