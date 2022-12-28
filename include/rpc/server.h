#ifndef CRPC_SERVER_H
#define CRPC_SERVER_H

#include <string>
#include <memory>

#include <boost/asio.hpp>
#include "tcp_connection.h"
#include "dispatcher.h"

namespace crpc {
    class server {
    public:
        server(std::string const &address, unsigned short port);
        ~server();

        //! Starts synchronous (blocking) server loop
        void run();

        template <typename R, typename... Args>
        void bind(std::string name, R (*func)(Args...)) {
            dispatcher_->bind(name, func);
        }

    private:
        //! Attach connection handlers
        void start_accept();

    private:
        boost::asio::io_service io_;
        boost::asio::ip::tcp::acceptor acceptor_;
        boost::asio::ip::tcp::socket socket_;
        std::shared_ptr<dispatcher> dispatcher_;
    };
}

#endif //CRPC_SERVER_H
