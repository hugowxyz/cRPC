#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <msgpack.hpp>

#include "rpc/response.h"

using namespace std;
using namespace crpc;
using namespace chrono_literals;

int main() {
    unordered_map<int, promise<msgpack::object>> m;
    promise<msgpack::object> p;
    future<msgpack::object> ftr = p.get_future();

//    msgpack::object_handle oh;

    m.insert({0, move(p)});

    thread t([&m]() {
        this_thread::sleep_for(2s);
        auto &p = m[0];
        msgpack::type::tuple<int, std::string> src(1, "example");

        // serialize the object into the buffer.
        // any classes that implements write(const char*,size_t) can be a buffer.
        std::stringstream buffer;
        msgpack::pack(buffer, src);

        // send the buffer ...
        buffer.seekg(0);

        // deserialize the buffer into msgpack::object instance.
        std::string str(buffer.str());

        msgpack::object_handle oh = msgpack::unpack(str.data(), str.size());

        // deserialized object is valid during the msgpack::object_handle instance is alive.
        msgpack::object deserialized = oh.get();
        response resp = response(oh.get());
        p.set_value(resp.result());
    });

    msgpack::object res = ftr.get();
    cout << res << endl;

    t.join();
}