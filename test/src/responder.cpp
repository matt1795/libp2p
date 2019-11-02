// daytime server for learning asio

#include "libp2p/connection/connection.hpp"
#include "libp2p/multiplex/raw-channel.hpp"
#include "libp2p/security/plaintext.hpp"
#include "libp2p/util/util.hpp"

#include <asio.hpp>
#include <multiformats/varint.hpp>

#include <chrono>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <string>

std::string make_daytime_response() {
    std::time_t now = std::time(0);
    return std::ctime(&now);
}

template <typename Transport, typename SecurityLayer, typename MultiplexLayer>
class ConnectionManager {
    using Connection =
        Libp2p::Connection<Transport, SecurityLayer, MultiplexLayer>;
    asio::io_context& io_context;
    asio::ip::tcp::acceptor acceptor;
    std::list<Connection> connection_pool;

    void start_accept() {
        acceptor.async_accept([this](auto const& error, Transport transport) {
            if (!error) {
                try {
                    connection_pool.emplace_back(std::move(transport));
                } catch (std::exception const& e) {
                    std::cerr
                        << "failed to setup libp2p connection: " << e.what()
                        << std::endl;
                }
                std::cout << "connection pool size: " << connection_pool.size()
                          << std::endl;
            }

            start_accept();
        });
    }

  public:
    ConnectionManager(asio::io_context& io_context)
        : io_context(io_context)
        , acceptor(io_context,
                   asio::ip::tcp::endpoint{asio::ip::tcp::v4(), 4002}) {
        start_accept();
    }
};

using SecurityLayer = Libp2p::Layer<Libp2p::Plaintext>;
using MuxerLayer = Libp2p::Layer<Libp2p::RawChannel>;

int main() {
    try {
        asio::io_context io_context;
        ConnectionManager<asio::ip::tcp::socket, SecurityLayer, MuxerLayer>
            connection_manager(io_context);
        io_context.run();
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
}
