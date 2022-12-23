#include <chrono>
#include <iostream>
#include <boost/asio.hpp>

int main() {
    boost::asio::io_context io;
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
