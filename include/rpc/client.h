#ifndef CRPC_CLIENT_H
#define CRPC_CLIENT_H

#define FMT_HEADER_ONLY
#include "fmt/core.h"

#include <string>
#include <msgpack.hpp>
#include <boost/asio.hpp>
#include <future>
#include <atomic>
#include <blockingconcurrentqueue.h>

using namespace boost::asio;
using boost::asio::ip::tcp;

namespace crpc {
    class client {
    public:
        client(std::string addr, uint16_t port);
        ~client();

        std::future<msgpack::object> test(int i);

        template <typename... Args>
        std::future<msgpack::object> call(std::string name, Args... args);

    private:
        //! Asynchronously connect to the endpoint
        void do_connect();
        void do_read();
        void start_write_thread();
        void async_run();

    private:
        io_service io_;
        ip::tcp::socket socket_;
        std::string addr_;
        uint16_t port_;

        std::thread io_thread_, write_thread_, read_thread_;

        //! Is the client currently ending connection
        std::atomic<bool> connected_;
        std::atomic<bool> exiting_;

        unsigned int task_id_;
        msgpack::unpacker unpacker_;
        std::unordered_map<unsigned int, std::promise<msgpack::object>> current_tasks_;
        moodycamel::BlockingConcurrentQueue<msgpack::sbuffer> task_queue_;

        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;

        static const uint32_t default_buffer_size = 4096;
    };

    template<typename... Args>
    std::future<msgpack::object> client::call(std::string name, Args... args) {
        fmt::print("Calling function {}\n", name);
        auto args_object = std::make_tuple(args...);
        unsigned int task_id = task_id_++;
        auto call_object = std::make_tuple(task_id, name, args_object);
        msgpack::sbuffer buffer;
        msgpack::pack(buffer, call_object);
        std::promise<msgpack::object> p;
        std::future<msgpack::object> fut = p.get_future();
        current_tasks_.insert({task_id, move(p)});
        task_queue_.enqueue(move(buffer));
        return fut;
    }
}

#endif //CRPC_CLIENT_H
