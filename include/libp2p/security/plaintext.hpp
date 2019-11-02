/** plaintext Security Layer (Debugging only)
 *
 * @author: Matthew Knight
 * @filename: plaintext.hpp
 * @date: 2019-11-02
 */

#pragma once

#include <string_view>

namespace Libp2p {
    struct Plaintext {
        using State = int;
        static constexpr std::string_view name{"/plaintext/1.0.0"};
    };
} // namespace Libp2p
