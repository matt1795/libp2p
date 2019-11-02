// test program for prototyping and testing

#include <asio.hpp>
#include <multiformats/multiaddr.hpp>

#include "libp2p/connection/connection.hpp"
#include "libp2p/security/plaintext.hpp"
#include "libp2p/multiplex/raw-channel.hpp"
#include "libp2p/util/util.hpp"

#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <tuple>
#include <variant>

using SecurityLayer =
    Libp2p::Layer<Libp2p::Plaintext>;

using MuxerLayer =
    Libp2p::Layer<Libp2p::RawChannel>;

int main() {

    Multiformats::Multiaddr multiaddr{"/dns/localhost/tcp/4002"};

    try {
        asio::thread_pool thread_pool;
        Libp2p::Connection<asio::ip::tcp::socket, SecurityLayer, MuxerLayer>
            connection{thread_pool, "localhost", "4002"};

        thread_pool.join();
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
