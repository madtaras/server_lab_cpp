#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <array>
#include <string>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

io_service ioservice;
tcp::resolver resolv{ioservice};
tcp::socket tcp_socket{ioservice};
std::array<char, 4096> bytes;
char* command;
char* command_arg;

void write_handler(const boost::system::error_code &ec,
                   std::size_t bytes_transferred)
{}


void read_handler(const boost::system::error_code &ec,
                  std::size_t bytes_transferred)
{
    if (!ec)
    {
        std::cout.write(bytes.data(), bytes_transferred);
    }
}

void connect_handler(const boost::system::error_code &ec)
{
    if (!ec)
    {
        tcp_socket.async_read_some(buffer(bytes), read_handler);
        std::string tmp;

        if (command_arg) {
            tmp = command + std::string(" ") + command_arg;
        } else {
            tmp = command;
        }

        async_write(tcp_socket, buffer(tmp), write_handler);
    }
}

void resolve_handler(const boost::system::error_code &ec,
                     tcp::resolver::iterator it)
{
    if (!ec)
        tcp_socket.async_connect(*it, connect_handler);
}

int main(int argc, char* argv[])
{
    command = argv[2];
    if (argc == 4) {
        command_arg = argv[3];
    }
    tcp::resolver::query q{argv[1], "2014"};
    resolv.async_resolve(q, resolve_handler);
    ioservice.run();
}
