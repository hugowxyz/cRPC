set(CMAKE_CXX_FLAGS -pthread)

find_package(msgpack REQUIRED)
find_package(Boost REQUIRED COMPONENTS system thread chrono)

add_executable(boost_asio_async_server boost_asio/async_server/main.cpp)
add_executable(boost_asio_server boost_asio/server/main.cpp)
add_executable(boost_asio_client boost_asio/client/main.cpp)
add_executable(server server/main.cpp)
add_executable(client client/main.cpp)
add_executable(threading threading/main.cpp)

target_link_libraries(boost_asio_async_server "${Boost_LIBRARIES}" msgpackc-cxx)
target_link_libraries(boost_asio_server "${Boost_LIBRARIES}")
target_link_libraries(boost_asio_client "${Boost_LIBRARIES}")
target_link_libraries(server "${Boost_LIBRARIES}" cRPC)
target_link_libraries(client "${Boost_LIBRARIES}" cRPC)
target_link_libraries(threading cRPC)