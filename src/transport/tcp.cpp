/** TCP Transport
 *
 * @author Matthew Knight
 * @filename tcp.cpp
 * @date 2019-10-24
 */

#include "libp2p/transport/tcp.hpp"

#include <unistd.h>

#include <cstring>
#include <iostream>

namespace {
    static constexpr std::uint64_t tcp{0x6};

    static constexpr std::uint64_t ip4{0x4};
    static constexpr std::uint64_t ip6{0x29};
    static constexpr std::uint64_t dns{0x35};
    static constexpr std::uint64_t dns4{0x36};
    static constexpr std::uint64_t dns6{0x37};

    static constexpr std::array allowed{ip4, ip6, dns, dns4, dns6};
    static constexpr std::array ipv4{ip4, dns4};
    static constexpr std::array ipv6{ip6, dns6};

    bool valid_multiaddr(Multiformats::Multiaddr const& multiaddr) {
        if (multiaddr.size() < 2)
            return false;

        if (std::next(multiaddr.begin())->code != tcp)
            return false;

        bool accepted_protocol = std::any_of(
            allowed.cbegin(), allowed.cend(), [&multiaddr](auto const& elem) {
                return elem == multiaddr.front().code;
            });

        if (!accepted_protocol)
            return false;

        return true;
    }

    // make hints for specific multiaddr
    auto make_hints(Multiformats::Multiaddr const& multiaddr) {
        struct addrinfo hints {
            .ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM, .ai_protocol = 0
        };

        if (!valid_multiaddr(multiaddr))
            throw std::invalid_argument("invalid address");

        if (multiaddr.front().code != dns) {
            bool ipv4_specific = std::any_of(
                ipv4.cbegin(), ipv4.cend(), [&multiaddr](auto const& elem) {
                    return elem == multiaddr.front().code;
                });

            bool ipv6_specific = std::any_of(
                ipv6.cbegin(), ipv6.cend(), [&multiaddr](auto const& elem) {
                    return elem == multiaddr.front().code;
                });

            if (ipv4_specific)
                hints.ai_family = AF_INET;
            else if (ipv6_specific)
                hints.ai_family = AF_INET6;
        }

        return hints;
    }

    template <typename Func>
    int make_socket(std::string const& node, std::string const& port,
                    addrinfo const& hints, Func func) {
        addrinfo* info;
        int fd;

        int ret = getaddrinfo(node.c_str(), port.c_str(), &hints, &info);
        if (ret == EAI_SYSTEM)
            throw std::system_error(errno, std::generic_category(),
                                    "getaddrinfo");
        else if (ret < 0)
            throw std::runtime_error(gai_strerror(ret));

        for (auto ptr = info; fd == -1 && ptr != NULL; ptr = ptr->ai_next) {
            fd = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
            if (fd == -1)
                continue;

            ret = func(fd, ptr->ai_addr, ptr->ai_addrlen);
            if (ret != -1)
                break;

            close(fd);
            fd = -1;
        }

        freeaddrinfo(info);

        if (fd == -1)
            throw std::system_error(errno, std::generic_category(),
                                    "ctor");

        return fd;
    }

    template <typename Func>
    int make_socket(Multiformats::Multiaddr const& multiaddr,
                    addrinfo const& hints, Func func) {

        if (!valid_multiaddr(multiaddr))
            throw std::invalid_argument("invalid address");

        return make_socket(multiaddr.begin()->to_string(),
                           std::next(multiaddr.begin())->to_string(), hints,
                           func);
    }

    int bind_listener(int fd, sockaddr* addr, socklen_t len) {
        int option;
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option,
                       sizeof(option)) < 0)
            throw std::system_error(errno, std::generic_category(),
                                    "setsockopt");

        return bind(fd, addr, len);
    }

    struct addrinfo listen_hints {
        .ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM, .ai_protocol = 0,
        .ai_flags = AI_PASSIVE
    };
} // namespace

namespace Libp2p {
    Tcp::Tcp(int fd)
        : fd(fd)
        , initiator(false) {
        if (fd < 0)
            throw std::runtime_error("failed making socket");
    }

    Tcp::Tcp(Multiformats::Multiaddr const& multiaddr)
        : fd(make_socket(multiaddr, make_hints(multiaddr), ::connect))
        , initiator(true) {
        if (fd < 0)
            throw std::runtime_error("failed making socket");
    }

    Tcp::~Tcp() { close(fd); }

    bool Tcp::initiated() const { return initiator; }

    void Tcp::shutdown() {
        if (::shutdown(fd, SHUT_RDWR) < 0)
            throw std::system_error(errno, std::generic_category(), "shutdown");
    }

    Tcp::Listener::Listener(std::string const& port)
        : fd(make_socket("*", port, listen_hints, bind_listener)) {

        if (::listen(fd, 0) < 0)
            throw std::system_error(errno, std::generic_category(), "listen");
    }

    Tcp::Listener::~Listener() { close(fd); }

    // TODO: what to do when not blocking? std::optional? do we need
    // non-blocking at all?
    Tcp Tcp::Listener::accept() { return {::accept(fd, nullptr, nullptr)}; }
} // namespace Libp2p
