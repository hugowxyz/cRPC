#ifndef CRPC_TCP_CONNECTION_H
#define CRPC_TCP_CONNECTION_H

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#define FMT_HEADER_ONLY
#include "fmt/core.h"

#include <msgpack.hpp>

using namespace std;
using namespace boost;
using asio::ip::tcp;

namespace rpc {
    class tcp_connection
        : public boost::enable_shared_from_this<tcp_connection> {
    public:
        typedef boost::shared_ptr<tcp_connection> pointer;

        static pointer create(boost::asio::io_service &io) {
            return pointer(new tcp_connection(io));
        }

        //! action on connection
        void start();

        tcp::socket& socket() {
            return socket_;
        }

    private:
        explicit tcp_connection(boost::asio::io_service &io)
        : socket_(io)
        , unpacker_() {
            unpacker_.reserve_buffer(default_buffer_size);
        }

    private:
        msgpack::unpacker unpacker_;
        msgpack::sbuffer output_buffer_;
        tcp::socket socket_;
        static const uint32_t default_buffer_size = 4096;
    };
}

#endif //CRPC_TCP_CONNECTION_H
