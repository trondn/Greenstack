add_executable(bitfield_pack_test bitfield_pack_test.cc)
add_test(NAME Bitfield-pack-test COMMAND bitfield_pack_test)

add_executable(flexheader_test
   flexheader_test.cc)
target_link_libraries(flexheader_test greenstack)
add_test(NAME Flex-header-test COMMAND flexheader_test)

add_executable(request_test request_test.cc)
target_link_libraries(request_test greenstack)
add_test(NAME Request-test COMMAND request_test)

add_executable(frame_test frame_test.cc)
target_link_libraries(frame_test greenstack)
add_test(NAME Frame-test COMMAND frame_test)

add_executable(encodedecode_test encodedecode_test.cc)
target_link_libraries(encodedecode_test greenstack)
add_test(NAME Encode-Decode-test COMMAND encodedecode_test)

add_executable(messagestream_test messagestream_test.cc)
target_link_libraries(messagestream_test greenstack)
add_test(NAME Messagestream-test COMMAND messagestream_test)

add_executable(ByteArrayReaderTest ByteArrayReaderTest.cc)
target_link_libraries(ByteArrayReaderTest greenstack_platform)
add_test(NAME ByteArrayReaderTest COMMAND ByteArrayReaderTest)

add_executable(ByteArrayWriterTest ByteArrayWriterTest.cc)
target_link_libraries(ByteArrayWriterTest greenstack_platform)
add_test(NAME ByteArrayWriterTest COMMAND ByteArrayWriterTest)
