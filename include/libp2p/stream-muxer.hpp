// libp2p Stream Muxer Interface
//
// Reference: https://github.com/libp2p/interface-stream-muxer
// Author: Matthew Knight
// File Name: connection.hpp
// Date: 2019-09-12

#pragma once

namespace Libp2p {
    class Muxer {
        Muxer(Connection, isListener);

        new_stream(/* function(err ,stream) */);
        new_stream();
        on(/* stream, function(err) */);
    };
}
