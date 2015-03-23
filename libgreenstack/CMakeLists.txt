add_library(greenstack SHARED
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/visibility.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Greenstack.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Buffer.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/FlexHeader.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Frame.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Request.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Response.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/Message.h
            ${Greenstack_SOURCE_DIR}/include/libgreenstack/MessageStream.h
            Reader.h
            Writer.h
            FlexHeader.cc
            Frame.cc
            Request.cc
            Response.cc
            Message.cc
            MessageStream.cc)
if (WIN32)
   target_link_libraries(greenstack greenstack_platform ws2_32)
else (WIN32)
   target_link_libraries(greenstack greenstack_platform)
endif (WIN32)