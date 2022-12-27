#include "rpc/client.h"

#include <iostream>
#include <msgpack.hpp>
#include <future>

using namespace std;
using namespace crpc;

int main() {
    client client("127.0.0.1", 3000);
    future<msgpack::object> fut = client.test();
    fut.wait();
    msgpack::object res = fut.get();
    cout << "nooo way " << res << "\n";
    cout << "WHAT" << endl;

//    client.call("add", 2, 3).get();
//    int result_value = -1;
//    result.convert(&result_value);
//    std::cout << "The result is: " << result_value << std::endl;
    return 0;
}