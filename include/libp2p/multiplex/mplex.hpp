/** mplex Multiplexer
 *
 * @author: Matthew Knight
 * @filename: mplex.hpp
 * @date: 2019-11-02
 */

#pragma once

#include <string_view>

namespace Libp2p {
    struct Mplex {
        using State = int;
        static constexpr std::string_view name{"/mplex/1.0.0"};
    };
} // namespace Libp2p
