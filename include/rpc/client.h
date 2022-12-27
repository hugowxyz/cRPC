#ifndef CRPC_CLIENT_H
#define CRPC_CLIENT_H

#include <string>
#include <msgpack.hpp>
#include <boost/asio.hpp>
#include <future>
#include <atomic>
#include <blockingconcurrentqueue.h>

using namespace std;
using namespace boost::asio;
using boost::asio::ip::tcp;

namespace crpc {
    class client {
    public:
        client(string addr, uint16_t port);
        ~client();

        future<msgpack::object> test(int i);

        template <typename... Args>
        future<msgpack::object> call(string name, Args... args);

    private:
        //! Asynchronously connect to the endpoint
        void do_connect();
        void do_read();
        void start_write_thread();
        void start_read_thread();
        void async_run();

    private:
        io_service io_;
        ip::tcp::socket socket_;
        string addr_;
        uint16_t port_;

        thread io_thread_, write_thread_, read_thread_;

        //! Is the client currently ending connection
        atomic<bool> exiting_;

        unsigned int task_id_;
        msgpack::unpacker unpacker_;
        unordered_map<unsigned int, promise<msgpack::object>> current_tasks_;
        moodycamel::BlockingConcurrentQueue<msgpack::sbuffer> task_queue_;

        static const uint32_t default_buffer_size = 4096;
    };

}

#endif //CRPC_CLIENT_H
