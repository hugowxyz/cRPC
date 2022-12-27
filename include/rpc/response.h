#ifndef CRPC_RESPONSE_H
#define CRPC_RESPONSE_H

#include <msgpack.hpp>

using namespace std;

namespace crpc {
    class response {
    public:
        response(const msgpack::object &deserialized);

        unsigned int id();
        msgpack::object result();

        using response_type = tuple<unsigned int, msgpack::object>;
    private:
        uint32_t id_;
        unique_ptr<msgpack::object> result_;
    };
}

#endif //CRPC_RESPONSE_H
