#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query("localhost", "3000");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
//        tcp::resolver::iterator end;
//        while (endpoint_iterator != end) {
//            tcp::endpoint endpoint = *endpoint_iterator++;
//            cout << endpoint << endl;
//        }

        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);

        for (;;)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::cout.write(buf.data(), len);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
