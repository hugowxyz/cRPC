#ifndef CRPC_SERVER_H
#define CRPC_SERVER_H

#include <string>

#include <boost/asio.hpp>
#include "tcp_connection.h"

using namespace boost::asio;

namespace rpc {
    class server {
    public:
        server(std::string const &address, unsigned short port);
        ~server();

        //! Starts synchronous (blocking) server loop
        void run();

    private:
        //! Attach connection handlers
        void start_accept();

    private:
        io_service io_;
        ip::tcp::acceptor acceptor_;
        ip::tcp::socket socket_;
    };
}

#endif //CRPC_SERVER_H
