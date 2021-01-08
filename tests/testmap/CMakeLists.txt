file(GLOB SRCS *.cpp)

ADD_EXECUTABLE(testmap ${SRCS})

TARGET_LINK_LIBRARIES(testmap
        libgtest
        libgmock
        )

add_test(NAME testmap
        COMMAND testmap)