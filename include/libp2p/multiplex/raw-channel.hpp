/** Raw channel -- No Multiplexer
 *
 * @author: Matthew Knight
 * @filename: raw-channel.hpp
 * @date: 2019-11-02
 */

#pragma once

#include <string_view>

namespace Libp2p {
    struct RawChannel {
        using State = int;
        static constexpr std::string_view name{"/raw-channel/1.0.0"};
    };
} // namespace Libp2p
