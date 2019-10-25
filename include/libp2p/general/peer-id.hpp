/**
 * PeerId
 *
 * @file peer-id.hpp
 * @author Matthew Knight
 * @date 2019-09-12
 * */

#pragma once

#include "multiformats/cid.hpp"

#include <string>
#include <vector>

namespace Libp2p {
    class PeerId {
        enum class KeyType {
            Rsa,
            Ed25519,
            Secp256k1,
            Ecdsa
        };

        struct Key {
            KeyType key_type;
            std::vector<std::uint8_t> data;
        };

        Key private_key;
        Key public_key;

        public:
        /** @brief Generate new peer-id */
        PeerId(KeyType key_type = KeyType::Rsa);

        /** @brief Convert public key to CID */
        Multiformats::Cid to_cid();
    };
}
