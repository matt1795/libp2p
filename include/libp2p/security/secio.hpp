/** secio Security Layer
 *
 * @author: Matthew Knight
 * @filename: secio.hpp
 * @date: 2019-11-02
 */

#pragma once

#include <string_view>

namespace Libp2p {
    struct Secio {
        using State = int;
        static constexpr std::string_view name{"/secio/1.0.0"};
    };
} // namespace Libp2p
