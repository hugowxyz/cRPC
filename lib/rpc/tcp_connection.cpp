#include "rpc/tcp_connection.h"

namespace rpc {
    void tcp_connection::start() {
        fmt::print("tcp connection start\n");
        auto self(shared_from_this());
        size_t length = 4096;
        unsigned char buf[length];
        fmt::print("before socket async read\n");
        socket_.async_read_some(
                boost::asio::buffer(buf, length),
                [this, self, &buf](const boost::system::error_code &error, size_t bytes) {
                    if (!error) {
                        string message(buf, buf + bytes);
                        fmt::print("bytes read {}\n", bytes);
                        asio::write(socket_, asio::buffer(message));
                    }
                });
    }
}