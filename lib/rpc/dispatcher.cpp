#include "rpc/dispatcher.h"

namespace crpc {
    template<typename R, typename... Args>
    void dispatcher::bind(std::string name, std::function<R(Args...)> func) {
        functions_.insert({
            name,
            [this, func](msgpack::object args_) -> msgpack::object {
                msgpack::type::tuple<Args...> args;
                auto res = apply(func, convert(args));
                msgpack::zone z;
                return msgpack::object(res, z);
            }
        });
    }

    response dispatcher::dispatch(const msgpack::object &unpacked) {
        client_message task;
        unpacked.convert(task);

        auto id = std::get<0>(task);
        auto name = std::get<1>(task);
        auto args = std::get<2>(task);

        auto it = functions_.find(name);
        if (it != functions_.end()) {
            auto res = (it->second)(args);
            return response::make_response(id, res);
        } else {
            msgpack::zone z;
            return response::make_response(id, msgpack::object("something went wrong", z));
        }
    }
}