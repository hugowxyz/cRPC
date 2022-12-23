#ifndef CRPC_CLIENT_H
#define CRPC_CLIENT_H

#include <string>

namespace crpc {
    class client {
    public:
        client(std::string const &addr, uint16_t port);
    };
}

#endif //CRPC_CLIENT_H
