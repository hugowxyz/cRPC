find_package(Boost REQUIRED COMPONENTS chrono)
find_package(Boost REQUIRED COMPONENTS system thread)

add_executable(boost_asio_async_server boost_asio_async_server/main.cpp)
add_executable(boost_asio_server boost_asio_server/main.cpp)
add_executable(boost_asio_client boost_asio_client/main.cpp)
add_executable(server server/main.cpp)

target_link_libraries(boost_asio_async_server "${Boost_LIBRARIES}")
target_link_libraries(boost_asio_server "${Boost_LIBRARIES}")
target_link_libraries(boost_asio_client "${Boost_LIBRARIES}")
target_link_libraries(server "${Boost_LIBRARIES}" cRPC)