#define FMT_HEADER_ONLY
#include "fmt/core.h"

#include <boost/asio.hpp>
#include <iostream>

#include "rpc/tcp_connection.h"
#include "rpc/server.h"

using namespace std;
using boost::asio::ip::tcp;

namespace crpc {
    server::server(const std::string &address, unsigned short port)
        : io_()
        , acceptor_(io_, tcp::endpoint(boost::asio::ip::address::from_string(address), port))
        , socket_(io_)
        , dispatcher_(std::make_shared<dispatcher>()) {
        fmt::print("Starting server on address {}:{}\n", address, port);
        start_accept();
    }

    server::~server() = default;

    void server::run() {
        io_.run();
    }

    void server::start_accept() {
        tcp_connection::pointer new_connection = tcp_connection::create(io_, dispatcher_);

        acceptor_.async_accept(
                new_connection->socket(),
                [this, new_connection](const boost::system::error_code &error) {
                    if (!error) {
                        fmt::print("Accepted connection\n");
                        new_connection->start();
                    }

                    start_accept();
                });
    }
}