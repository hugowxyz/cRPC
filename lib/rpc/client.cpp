#include "rpc/client.h"

#define FMT_HEADER_ONLY
#include "fmt/core.h"

#include <boost/asio.hpp>
#include <utility>
#include <iostream>

using namespace std;
using boost::asio::ip::tcp;

namespace crpc {
    client::client(string addr, uint16_t port)
        : io_()
        , socket_(io_)
        , addr_(move(addr))
        , port_(port)
        , exiting_(false)
        , task_id_(0) {

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

        do_connect(); // blocking
        start_write_thread();
//        start_read_thread();
        async_run();
    }

    client::~client() {
        fmt::print("Called from client destructor\n");
        exiting_ = true;
        io_thread_.join();
        read_thread_.join();
        write_thread_.join();
    }

    void client::do_connect() {
        tcp::resolver resolver(io_);
        auto endpoint_iterator = resolver.resolve({addr_, to_string(port_)});
        boost::asio::connect(socket_, endpoint_iterator);

        do_read();
    }

    void client::do_read() {
        unsigned char c[default_buffer_size];
        socket_.async_read_some(
                boost::asio::buffer(c, default_buffer_size),
                [this, &c](boost::system::error_code error, size_t bytes) {
                    fmt::print("Read {} bytes\n", bytes);
                    if (!error) {
                        string message(c, c + bytes);
                        fmt::print("Message: {}\n", message);
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

                boost::asio::write(
                        socket_,
                        boost::asio::buffer(task.data(), task.size()));
            }

            fmt::print("Exiting write thread...\n");
        });

        write_thread_ = move(write_thread);
    }

    void client::start_read_thread() {
        thread read_thread([this]() {
            fmt::print("Starting read thread...\n");
            unsigned char c[default_buffer_size];
            msgpack::unpacker unpacker;
            boost::system::error_code error;
            while(!exiting_) {
                fmt::print("Waiting for input..\n");
                size_t bytes = socket_.read_some(boost::asio::buffer(c, default_buffer_size));
                fmt::print("Received {} bytes\n", bytes);
                if (error == boost::asio::error::eof) {
                    break;
                } else if (error) {
                    fmt::print("Error receiving data: {}", error.message());
                    throw boost::system::system_error(error);
                }

                promise<msgpack::object> &promise = current_tasks_[1];

                msgpack::type::tuple<int, bool, std::string> src(1, true, "example");

                // serialize the object into the buffer.
                // any classes that implements write(const char*,size_t) can be a buffer.
                std::stringstream buffer;
                msgpack::pack(buffer, src);

                // send the buffer ...
                buffer.seekg(0);

                // deserialize the buffer into msgpack::object instance.
                std::string str(buffer.str());

                msgpack::object_handle oh =
                        msgpack::unpack(str.data(), str.size());

                // deserialized object is valid during the msgpack::object_handle instance is alive.
                msgpack::object deserialized = oh.get();

                cout << "des " << deserialized << endl;

                promise.set_value(deserialized);

//                unpacker.buffer_consumed(bytes);
//                msgpack::unpacked result;
//                while (unpacker.next(result)) {
//                    msgpack::object deserialized = result.get();
//                    cout << "Deserialized object: " << deserialized << endl;
//                }
            }

            fmt::print("Exiting read thread...\n");
        });

        read_thread_ = move(read_thread);
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
        auto args_object = make_tupe(args...);
        int task_id = task_id_++;
        auto call_object = make_tuple(task_id, name, args_object);
        msgpack::sbuffer buffer;
        msgpack::pack(buffer, call_object);
//        promise<msgpack::object> p();
        current_tasks_.insert({task_id, promise<msgpack::object>()});
        task_queue_.enqueue(move(buffer));
        return {}; // what??
    }

    future<msgpack::object> client::test() {
        string message = "hehehhe";
        msgpack::type::tuple<int, string> src(1, message);
        msgpack::sbuffer buf;
        msgpack::pack(buf, src);

        promise<msgpack::object> p;
        future<msgpack::object> ftr = p.get_future();
        current_tasks_.insert({1, move(p)});
        boost::asio::write(socket_, boost::asio::buffer(buf.data(), buf.size()));
        return ftr;
    }
}


