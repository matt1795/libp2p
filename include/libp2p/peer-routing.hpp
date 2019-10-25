// libp2p Peer Routing Interface
//
// Reference: https://github.com/libp2p/interface-peer-routing
// Author: Matthew Knight
// File Name: connection.hpp
// Date: 2019-09-12

#pragma once

namespace Libp2p {
    class PeerRouting {
        find_peers(/* key, callback(err, peersPriorityQueue) */);
    };
}
