/** yamux Multiplexer
 *
 * @author: Matthew Knight
 * @filename: yamux.hpp
 * @date: 2019-11-02
 */

#pragma once

#include <string_view>

namespace Libp2p {
    struct Yamux {
        using State = int;
        static constexpr std::string_view name{"/yamux/1.0.0"};
    };
} // namespace Libp2p
