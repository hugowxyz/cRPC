#include "rpc/server.h"

#include <tuple>

using namespace crpc;

std::string lolol(std::string s) {
    return s;
}

int add(int a, int b) {
    return a + b;
}

std::tuple<int, std::string, std::string> nothing(std::string what) {
    return std::make_tuple(1, "haha", "c");
}

int main() {
    server s("127.0.0.1", 3000);
    s.bind("test", &lolol);
    s.bind("add", &add);
    s.bind("nothing", &nothing);
    s.run();
}
