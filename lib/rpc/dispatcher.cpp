#include "rpc/dispatcher.h"

#include <iostream>

namespace crpc {
    response dispatcher::dispatch(const msgpack::object &unpacked) {
        client_message task;
        unpacked.convert(task);

        auto id = std::get<0>(task);
        auto name = std::get<1>(task);
        auto args = std::get<2>(task);

        auto it = functions_.find(name);
        if (it != functions_.end()) {
            auto res = (it->second)(args);
            std::cout << "res: " << res << std::endl;
            return response::make_response(id, res);
        } else {
            msgpack::zone z;
            return response::make_response(id, msgpack::object("something went wrong", z));
        }
    }
}