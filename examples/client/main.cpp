#include "rpc/client.h"
#include "rpc/utils.h"

#include <iostream>
#include <msgpack.hpp>
#include <future>

using namespace std;
using namespace crpc;

int main() {
    client client("127.0.0.1", 3000);
    auto res1 = client.call("test", "arg").get();
    auto res2 = client.call("add", 1, 2).get();
//    auto res3 = client.call("nothing", "arg").get();
    string res1_val;
    res1.convert(res1_val);
    cout << "res1 " << res1_val << endl;
    int res2_val;
    res2.convert(res2_val);
    cout << "res2 " << res2_val << endl;
//    using ret_type = tuple<int, std::string, std::string>;
//    ret_type res3_val;
//    res3.convert(res3_val);
//    utils::print(res3_val); cout << endl;
    return 0;
}