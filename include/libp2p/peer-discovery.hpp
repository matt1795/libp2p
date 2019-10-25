// libp2p Peer Discovery Interface
//
// Reference: https://github.com/libp2p/interface-peer-discovery
// Author: Matthew Knight
// File Name: connection.hpp
// Date: 2019-09-12

#pragma once

namespace Libp2p {
    class Discovery {
        start(/* callback */);
        stop(/* callback */);
        on('peer', /* callback */);
    };
}
