#ifndef CRPC_TCP_CONNECTION_H
#define CRPC_TCP_CONNECTION_H

#include <memory>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#define FMT_HEADER_ONLY
#include "fmt/core.h"
#include "dispatcher.h"

#include <msgpack.hpp>
#include <utility>

using boost::asio::ip::tcp;

namespace crpc {
    class tcp_connection
        : public boost::enable_shared_from_this<tcp_connection> {
    public:
        typedef boost::shared_ptr<tcp_connection> pointer;

        static pointer create(boost::asio::io_service &io, std::shared_ptr<dispatcher> dispatcher) {
            return pointer(new tcp_connection(io, std::move(dispatcher)));
        }

        //! action on connection
        void start();

        tcp::socket& socket() {
            return socket_;
        }

    private:
        explicit tcp_connection(boost::asio::io_service &io, std::shared_ptr<dispatcher> dispatcher)
        : socket_(io)
        , unpacker_()
        , dispatcher_(dispatcher) {
            unpacker_.reserve_buffer(default_buffer_size);
        }

    private:
        static const uint32_t default_buffer_size = 4096;

        msgpack::unpacker unpacker_;
        msgpack::sbuffer output_buffer_;
        tcp::socket socket_;
        std::shared_ptr<dispatcher> dispatcher_;
    };
}

#endif //CRPC_TCP_CONNECTION_H
