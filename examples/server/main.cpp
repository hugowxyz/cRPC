#include "rpc/server.h"

using namespace std;
using namespace crpc;

string test(string s) {
    return s;
}

int main() {
    server s = server("127.0.0.1", 3000);
    s.bind("test", test);
    s.run();
}
