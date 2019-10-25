/** TCP Transport
 *
 * @author Matthew Knight
 * @filename tcp.hpp
 * @date 2019-10-24
 */

#pragma once

#include <multiformats/multiaddr.hpp>

#include <algorithm>
#include <system_error>

#include <netdb.h>
#include <sys/socket.h>

#include <iostream>

namespace Libp2p {
    class Tcp {
        int fd{-1};
        bool initiator;

      public:
        static constexpr bool secure = false;
        static constexpr bool multiplexed = false;

        struct Listener {
            int fd = -1;

          public:
            /** @brief Construct listener bound to a port */
            Listener(std::string const& port);

            ~Listener();

            /** @brief Accept connection */
            Tcp accept();
        };

        /** @brief Contruct from accepted connection */
        Tcp(int fd);

        /** @brief Construct by connecting to a multiaddr */
        Tcp(Multiformats::Multiaddr const& multiaddr);

        ~Tcp();

        /** @brief Whether the transport initiate the connection */
        bool initiated() const;

        /** @brief Write to transport */
        template <typename InputIterator>
        void send(InputIterator begin, InputIterator end) const {
            std::array<std::uint8_t, 256> buf;

            if (std::distance(begin, end) < 0)
                throw std::invalid_argument("iterators not in order");

            while (begin != end) {
                std::size_t n = std::min(
                    static_cast<std::size_t>(std::distance(begin, end)),
                    buf.size());
                std::copy_n(begin, n, buf.begin());
                ssize_t sent = ::send(fd, buf.data(), n, 0);

                if (sent < 0)
                    throw std::system_error(errno, std::generic_category(),
                                            "send");
                begin += sent;
            }
        }

        /** @brief Read from transport */
        template <typename OutputIterator>
        void receive(OutputIterator out) const {
            std::array<std::uint8_t, 256> buf;
            ssize_t n;

            do {
                n = recv(fd, buf.data(), buf.size(), MSG_DONTWAIT);
                if (n < 0)
                    throw std::system_error(errno, std::generic_category(),
                                            "send");

                std::copy_n(buf.cbegin(), n, out);
            } while (n == buf.size());
        }

        /** @brief disable read and write operations */
        void shutdown();
    };
} // namespace Libp2p
