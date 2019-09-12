// libp2p Connection Interface
//
// Reference: https://github.com/libp2p/interface-connection
// Author: Matthew Knight
// File Name: connection.hpp
// Date: 2019-09-12

#pragma once

namespace Libp2p {
    class Connection {
        get_observed_addrs(/* callback */);
        get_peer_info(/* callback */);
        set_peer_info(/* peer-info */);
    };
}
