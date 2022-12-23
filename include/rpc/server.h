//
// Created by phanh on 23/12/2022.
//

#ifndef CRPC_SERVER_H
#define CRPC_SERVER_H

#include <string>

namespace rpc {
    class server {
        server(std::string const &address, uint16_t port);
    };
}

#endif //CRPC_SERVER_H
