#include "rpc/server.h"

#include <iostream>

#define FMT_HEADER_ONLY
#include "fmt/core.h"

using namespace std;
using boost::asio::ip::tcp;

namespace rpc {
    server::server(const std::string &address, unsigned short port)
        : io_()
        , acceptor_(io_, ip::tcp::endpoint(ip::address::from_string(address), port))
        , socket_(io_) {
        fmt::print("Starting server on address {}:{}\n", address, port);
    }

    server::~server() = default;

    [[noreturn]] void server::run() {
        for (;;) {
            fmt::print("Waiting for connection...\n");
            tcp::socket socket(io_);
            acceptor_.accept(socket);

            string message = "Some message!";

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
            fmt::print("Sent message: {}\n", message);
        }
    }

    void server::start_accept() {
    }
}