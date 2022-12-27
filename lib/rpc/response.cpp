#include "rpc/response.h"

namespace crpc {
    response::response(const msgpack::object &deserialized) {
        response_type res;
        deserialized.convert(res);
        id_ = get<0>(res);
        result_ = make_unique<msgpack::object>(get<1>(res));
    }

    unsigned int response::id() {
        return id_;
    }

    msgpack::object response::result() {
        return *result_;
    }
}
