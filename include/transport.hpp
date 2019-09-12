// libp2p Transport Interface
//
// Reference: https://github.com/libp2p/interface-transport
// Author: Matthew Knight
// File Name: connection.hpp
// Date: 2019-09-12

#pragma once

using namespace Libp2p {
    class MultiaddrConnection {
        // streaming iterable sink
        sink();

        // streaming iterable source
        AsyncIterator source();

        ative_handle conn();
        Multiaddr remote_addr();
    };

    class Upgrader {
        public:
        upgrade_outbound(MultiaddrConnection const& conn);
        upgrade_inbound(MultiaddrConnection const& conn);
    };

    class Transport {
      public:
        class Listener {
          public:
            enum class Event { Listening, Close, Connection, Error };

            Promise listen(Multiaddr const& addr);
            get_addrs();
            Promise close(/* options */);
        };

        Transport(Upgrader const& upgrader /*, options */);
        virtual Promise dial(Multiaddr const& addr /*, options */) = 0;
        virtual void createListener(/* options, func */) = 0;
    };
}
