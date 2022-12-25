set(OUTPUT_LIBRARY_NAME ${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

add_library(${CMAKE_PROJECT_NAME}
        rpc/client.cpp
        rpc/server.cpp
        rpc/tcp_connection.cpp)

target_link_libraries(${OUTPUT_LIBRARY_NAME})