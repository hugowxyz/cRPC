#include "rpc/client.h"
#include "rpc/response.h"

#define FMT_HEADER_ONLY
#include "fmt/core.h"

#include <boost/asio.hpp>
#include <utility>
#include <iostream>

using boost::asio::ip::tcp;

namespace crpc {
    client::client(string addr, uint16_t port)
        : io_()
        , socket_(io_)
        , addr_(move(addr))
        , port_(port)
        , exiting_(false)
        , task_id_(0)
        , unpacker_() {
        /*
         * Launch io, reader and writer threads.
         * Cannot run io on main thread because it is blocking.
         *
         * 1. Connect to the endpoint and start reader thread
         * 2. Start writer thread
         * 3. Start io thread
         *
         * - The client calling a remote procedure will enqueue a task.
         * - This will be read by the writer thread, which will assign the task an ID.
         * and create a corresponding future. It will then send the serialized request (with ID) to the server.
         * - The server will execute the remote procedure before sending back the result (with ID). The reader
         * will set value for the corresponding future.
         *
         * 27/12/2022
         * Having a separate read thread causes io to terminate immediately, switching to async read on main thread.
         */
        unpacker_.reserve_buffer(default_buffer_size);
        do_connect(); // blocking
        async_run();
        start_write_thread();
    }

    client::~client() {
        fmt::print("Called from client destructor\n");
        exiting_ = true;
        io_.stop();
        // custom method
        task_queue_.cancel_wait();
//        read_thread_.join();
        write_thread_.join();
        io_thread_.join();
    }

    void client::do_connect() {
        tcp::resolver resolver(io_);
        auto endpoint_iterator = resolver.resolve({addr_, to_string(port_)});
        boost::asio::connect(socket_, endpoint_iterator);

        do_read();
    }

    void client::do_read() {
        socket_.async_read_some(
                boost::asio::buffer(unpacker_.buffer(), default_buffer_size),
                [this](boost::system::error_code error, size_t bytes) {
                    fmt::print("Read {} bytes\n", bytes);
                    if (!error) {
                        unpacker_.buffer_consumed(bytes);
                        msgpack::object_handle oh;
                        while (unpacker_.next(oh)) {
                            auto res = response(oh.get());
                            auto &promise = current_tasks_[res.id()];
                            promise.set_value(res.result());
                        }

                        do_read();
                    }
                });
    }

    void client::start_write_thread() {
        thread write_thread([this]() {
            fmt::print("Starting write thread...\n");
            msgpack::sbuffer task;
            while (!exiting_) {
                task_queue_.wait_dequeue(task);

                if (exiting_)
                    break;

                fmt::print("Sending data to server\n");
                boost::asio::write(
                        socket_,
                        boost::asio::buffer(task.data(), task.size()));
            }

            fmt::print("Exiting write thread...\n");
        });

        write_thread_ = move(write_thread);
    }

    void client::async_run() {
        thread io_thread([this]() {
            fmt::print("Starting io loop...\n");
            io_.run();
            fmt::print("Ending io loop...\n");
        });

        io_thread_ = move(io_thread);
    }

    template<typename... Args>
    future<msgpack::object> client::call(string name, Args... args) {
        fmt::print("Calling function {}\n", name);
        auto args_object = make_tuple(args...);
        unsigned int task_id = task_id_++;
        auto call_object = make_tuple(task_id, name, args_object);
        msgpack::sbuffer buffer;
        msgpack::pack(buffer, call_object);
        promise<msgpack::object> p;
        future<msgpack::object> fut = p.get_future();
        current_tasks_.insert({task_id, move(p)});
        task_queue_.enqueue(move(buffer));
        return fut;
    }

    future<msgpack::object> client::test(int i) {
        string message = "hehehhe";
        msgpack::type::tuple<int, string> src(i, message);
        msgpack::sbuffer buf;
        msgpack::pack(buf, src);

        promise<msgpack::object> p;
        future<msgpack::object> ftr = p.get_future();
        current_tasks_.insert({i, move(p)});
        boost::asio::write(socket_, boost::asio::buffer(buf.data(), buf.size()));
        return ftr;
    }
}


