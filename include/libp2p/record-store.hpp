// libp2p Peer Routing Interface
//
// Reference: https://github.com/libp2p/interface-record-store
// Author: Matthew Knight
// File Name: connection.hpp
// Date: 2019-09-12

#pragma once

namespace Libp2p {
    class RecordStore {
        get(/* key, callback(err, records) */);
        put(/* key, recordSignatureMultiHash, callback(err) */);
    };
}
