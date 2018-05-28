#include <boost/asio/io_service.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>
#include <ctime>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

io_service ioservice;
tcp::endpoint tcp_endpoint{tcp::v4(), 2014};
tcp::acceptor tcp_acceptor{ioservice, tcp_endpoint};
tcp::socket tcp_socket{ioservice};
std::string data;
std::array<char, 4096> bytes;

std::vector<tcp::socket> _sockets;
void accept_handler(const boost::system::error_code &ec);

std::string getCurrentDate()
{
    char cptime[50];
    time_t now = time(NULL);
    strftime(cptime, 50, "%b. %d, %Y", localtime(&now));
    return std::string(cptime);
}

void write_handler(const boost::system::error_code &ec,
                   std::size_t bytes_transferred)
{
    tcp_acceptor.listen();
    tcp_acceptor.async_accept(tcp_socket, accept_handler);
}


void read_handler(const boost::system::error_code &ec,
                  std::size_t bytes_transferred)
{
    if (!ec)
    {
        std::cout.write(bytes.data(), bytes_transferred);
        std::string tmp;
        std::string command (bytes.data());

        if (command == "d") {
            tmp = getCurrentDate() + "\n";
        } else if (command == "t") {
            std::time_t t = std::time(nullptr);
            char mbstr[100];
            std::strftime(mbstr, sizeof(mbstr), "%I:%M:%S", std::localtime(&t));
            tmp = (std::string) mbstr + "\n";
        } else if (command == "h") {
            tmp = "Hello\n";
        } else if (command == "m") {
            // TODO: handle message
            tmp = "other\n";
        }

        async_write(_sockets[_sockets.size() - 1], buffer(tmp), write_handler);
    }
}



void accept_handler(const boost::system::error_code &ec)
{
    if (!ec)
    {
        _sockets.push_back(std::move(tcp_socket));
        _sockets[_sockets.size() - 1].async_read_some(buffer(bytes), read_handler);
    }
}

int main()
{
    std::cout << "Server started" << std::endl;
    tcp_acceptor.listen();
    tcp_acceptor.async_accept(tcp_socket, accept_handler);
    ioservice.run();

}
