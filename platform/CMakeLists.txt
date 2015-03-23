# The platform directory contains implementations for functions
# I expect to be available on all platforms.

# The linker will complain if I try to create a library without
# any symbols, so I'll start off by creating a "dummy" symbol
# that won't be used by anyway
list(APPEND platform_specific_files dummy.c)

if (NOT HAVE_HTONLL)
   message(STATUS "Add implementation for ntohll/htonll")
   list(APPEND platform_specific_files ntohll.c)
endif (NOT HAVE_HTONLL)

add_library(greenstack_platform STATIC ${platform_specific_files})
if (WIN32)
   target_link_libraries(greenstack_platform ws2_32)
endif (WIN32)
