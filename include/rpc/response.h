#ifndef CRPC_RESPONSE_H
#define CRPC_RESPONSE_H

#include <msgpack.hpp>

namespace crpc {
    class response {
    public:
        response() = default;
        explicit response(const msgpack::object &deserialized);

        static response make_response(uint32_t id, msgpack::object obj) {
            response inst;
            inst.id_ = id;
            inst.result_ = std::make_unique<msgpack::object>(obj);
            return inst;
        }

        void write(msgpack::sbuffer *buf) {
            std::tuple<uint32_t, msgpack::object> r = std::make_tuple(id_, *result_);
            msgpack::pack(*buf, r);
        }

        unsigned int id();
        msgpack::object result();

        using response_type = std::tuple<unsigned int, msgpack::object>;
    private:
        uint32_t id_;
        std::unique_ptr<msgpack::object> result_;
    };
}

#endif //CRPC_RESPONSE_H
