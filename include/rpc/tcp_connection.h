#ifndef CRPC_TCP_CONNECTION_H
#define CRPC_TCP_CONNECTION_H

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#define FMT_HEADER_ONLY
#include "fmt/core.h"

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
        tcp_connection(boost::asio::io_service &io) : socket_(io) {}
//        void handle_write(const boost::system::error_code &error, size_t bytes) {}

    private:
        tcp::socket socket_;

    };
}

#endif //CRPC_TCP_CONNECTION_H
