/** tls Security Layer
 *
 * @author: Matthew Knight
 * @filename: yamux.hpp
 * @date: 2019-11-02
 */

#pragma once

#include <string_view>

namespace Libp2p {
    struct Tls {
        using State = int;
        static constexpr std::string_view name{"/tls/1.0.0"};
    };
} // namespace Libp2p
