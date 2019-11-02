// Utility classes and functions
//
// Author: Matthew Knight
// File Name: util.hpp
// Date: 2019-11-02

#pragma once

#include <variant>

namespace Libp2p {
    template <typename... Modules>
    struct Layer {
        using Protocols = std::tuple<Modules...>;
        using State = std::variant<std::monostate, typename Modules::State...>;
    };
} // namespace Libp2p
