#ifndef CRPC_DISPATCHER_H
#define CRPC_DISPATCHER_H

#include "response.h"

#include <unordered_map>
#include <string>

#include <msgpack.hpp>

namespace crpc {
    //! Maintain and call function bindings
    class dispatcher {
    public:
        template <typename R, typename... Args>
        void bind(std::string name, std::function<R(Args...)> func);

        response dispatch(const msgpack::object &unpacked);

        using client_message = std::tuple<uint32_t, std::string, msgpack::object>;
        using function_type = std::function<msgpack::object(msgpack::object const &)>;

    private:
        // https://en.cppreference.com/w/cpp/utility/integer_sequence
        template <template <typename...> class Params, typename... Args, std::size_t... I>
        auto convert_helper(const Params<Args...> &params, std::index_sequence<I...>) {
            return std::make_tuple(std::get<I>(params)...);
        }

        template <template <typename...> class Params, typename... Args>
        auto convert(const Params<Args...> &params) {
            return convert_helper(params, std::index_sequence_for<Args...>{});
        }

        std::unordered_map<std::string, function_type> functions_;
    };
}

#endif //CRPC_DISPATCHER_H
