#include "rpc/server.h"

using namespace rpc;

int main() {
    server s = server("127.0.0.1", 3000);
    s.run();
}
