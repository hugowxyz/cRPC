#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}

int main()
{
    try
    {
        boost::asio::io_service io_service;
        auto endpoint = tcp::endpoint(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 3000));
        tcp::acceptor acceptor(io_service, endpoint);
        cout << endpoint << endl;
        for (;;)
        {
            cout << "waiting for connection..." << endl;
            tcp::socket socket(io_service);
            acceptor.accept(socket);

            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
            cout << "sent message " << message << endl;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}