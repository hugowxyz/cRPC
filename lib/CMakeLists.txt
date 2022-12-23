set(OUTPUT_LIBRARY_NAME ${CMAKE_PROJECT_NAME})

add_library(${CMAKE_PROJECT_NAME}
        rpc/client.cpp
        rpc/server.cpp)

target_link_libraries(${OUTPUT_LIBRARY_NAME})