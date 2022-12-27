#include "rpc/server.h"

#include <boost/asio.hpp>

#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "rpc/tcp_connection.h"

using namespace std;
using boost::asio::ip::tcp;

namespace rpc {
    server::server(const std::string &address, unsigned short port)
        : io_()
        , acceptor_(io_, ip::tcp::endpoint(ip::address::from_string(address), port))
        , socket_(io_) {
        fmt::print("Starting server on address {}:{}\n", address, port);
        start_accept();
    }

    server::~server() = default;

    void server::run() {
        io_.run();
    }

    void server::start_accept() {
        tcp_connection::pointer new_connection = tcp_connection::create(io_);

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