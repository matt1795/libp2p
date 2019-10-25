// libp2p Connection Interface
//
// Reference: https://github.com/libp2p/interface-connection
// Author: Matthew Knight
// File Name: connection.hpp
// Date: 2019-09-12

#pragma once

namespace Libp2p {
    // secure and multiplexed
    template <typename Transport, typename Security, typename Multiplexer>
    class Connection {
        Transport transport;
        Security security;
        Multiplexer multiplexer;

    public:
        /** @brief Initiate connection to another host */
        Connection(Multiformats::Multiaddr const& multiaddr)
            : transport(multiaddr) {}

        /** @brief Create connection object from incoming */
        Connection(Transport::Responser&& responder) {}
    };
}
