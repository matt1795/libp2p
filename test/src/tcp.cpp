//
// Author: Matthew Knight
// File Name: tcp.cpp
// Date: 2019-10-21

#include "libp2p/transport/tcp.hpp"

#include <gtest/gtest.h>
#include <multiformats/multiaddr.hpp>

#include <netdb.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <iostream>
#include <thread>

#include <cstring>

class Epoll {
    int fd = -1;

  public:
    Epoll()
        : fd(epoll_create1(0)) {
        if (fd < -1)
            throw std::system_error(errno, std::generic_category(),
                                    "epoll_create1");
    }

    ~Epoll() { close(fd); }

    void add(int desc, struct epoll_event* event) {}
    void mod(int desc, struct epoll_event* event) {}
    void del(int desc, struct epoll_event* event) {}
    void wait() {}
};

TEST(TcpTests, Test) {
    try {
        Multiformats::Multiaddr multiaddr{"/ip4/127.0.0.1/tcp/4001"};
        std::string message{"hello"};
        std::string buf{""};

        Libp2p::Tcp::Listener listener("4001");
        Libp2p::Tcp client{multiaddr};
        auto server = listener.accept();
        server.send(message.begin(), message.end());
        client.receive(std::back_inserter(buf));

        std::cout << buf << std::endl;
    } catch (std::exception const& e) {
        std::cout << e.what() << std::endl;
    }
}
