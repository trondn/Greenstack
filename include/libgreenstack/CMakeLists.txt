add_subdirectory(core)
add_subdirectory(memcached)
install(FILES
        Buffer.h
        FlexHeader.h
        Frame.h
        Greenstack.h
        Message.h
        Opcodes.h
        Request.h
        Response.h
        Status.h
        visibility.h
        DESTINATION include/libgreenstack)
