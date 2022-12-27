#include "rpc/tcp_connection.h"
#include <msgpack.hpp>

#include <iostream>

using namespace std;

namespace rpc {
    void tcp_connection::start() {
        fmt::print("tcp connection start\n");
        auto self(shared_from_this());
        socket_.async_read_some(
                boost::asio::buffer(unpacker_.buffer(), default_buffer_size),
                [this, self](const boost::system::error_code &error, size_t bytes) {
                    if (!error) {
                        fmt::print("Read {} bytes\n", bytes);
                        unpacker_.buffer_consumed(bytes);
                        msgpack::object_handle oh;
                        while (unpacker_.next(oh)) {
                            output_buffer_.clear();
                            response task = oh.get();
                            cout << "received task " << task << endl;
                            msgpack::type::tuple<int, std::string> src(0, "example");
                            msgpack::pack(output_buffer_, src);
                            boost::asio::write(
                                    socket_,
                                    boost::asio::buffer(output_buffer_.data(), output_buffer_.size()));
                        }
                    }
                });
    }
}