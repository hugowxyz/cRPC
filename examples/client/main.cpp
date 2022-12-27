#include "rpc/client.h"

#include <iostream>
#include <msgpack.hpp>
#include <future>

using namespace std;
using namespace crpc;

int main() {
    client client("127.0.0.1", 3000);
    for (int i=0; i<5; i++) {
        msgpack::object res = client.test(i).get();
        cout << "Result: " << res << "\n";
    }

//    client.call("add", 2, 3).get();
//    int result_value = -1;
//    result.convert(&result_value);
//    std::cout << "The result is: " << result_value << std::endl;
    return 0;
}