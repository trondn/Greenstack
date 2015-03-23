file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Greenstack/payload)


macro(Flatbuffers)
   add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Greenstack/payload/${ARGV0}_generated.h
                      COMMAND
                         ${FLATC} -c
                                  -o ${CMAKE_CURRENT_BINARY_DIR}/Greenstack/payload
                                  --gen-includes
                                  ${CMAKE_CURRENT_SOURCE_DIR}/${ARGV0}.fbs
                      DEPENDS
                         ${CMAKE_CURRENT_SOURCE_DIR}/${ARGV0}.fbs
                      COMMENT "Generating code for ${ARGV0}")
    list(APPEND FLATBUFFER_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/Greenstack/payload/${ARGV0}_generated.h")
endmacro(Flatbuffers)

Flatbuffers(HelloRequest)
Flatbuffers(HelloResponse)
Flatbuffers(SaslAuthRequest)
Flatbuffers(SaslAuthResponse)
Flatbuffers(SelectBucketRequest)
Flatbuffers(ListBucketsResponse)
Flatbuffers(CreateBucketRequest)
Flatbuffers(DeleteBucketRequest)
Flatbuffers(AssumeRoleRequest)
Flatbuffers(Document)
Flatbuffers(MutationRequest)
Flatbuffers(MutationResponse)
FlatBuffers(GetRequest)
FlatBuffers(GetResponse)

add_custom_target(generate_flatbuffer_headers ALL
                  DEPENDS
                  ${FLATBUFFER_SOURCES})
