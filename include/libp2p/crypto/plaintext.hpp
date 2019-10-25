/** Plaintext Security Layer (AKA none.)
 *
 * @author Matthew Knight
 * @file plaintext.hpp
 * @date 2019-10-24
 */

#pragma once

namespace Libp2p {
    template <typename Transport>
    class Plaintext {
        public:
            Plaintext(Transport& transport);
    };
}
