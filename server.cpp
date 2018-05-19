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
    }
}



void accept_handler(const boost::system::error_code &ec)
{
    if (!ec)
    {
        _sockets.push_back(std::move(tcp_socket));

        // Send data to all clients to calc
        if (_sockets.size() == 3) {
            for(int i = 0; i < _sockets.size(); i++){
                std::string tmp = std::to_string(i) + "\n";
                async_write(_sockets[i], buffer(tmp), write_handler);

                _sockets[i].async_read_some(buffer(bytes), read_handler);
            }
        }
    }
}

int main()
{
    std::cout << "Server started" << std::endl;
    tcp_acceptor.listen();
    tcp_acceptor.async_accept(tcp_socket, accept_handler);
    ioservice.run();

}
