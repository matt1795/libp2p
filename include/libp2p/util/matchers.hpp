// Matcher Fuctions for validating byte streams
//
// Author Matthew Knight
// File Name matchers.hpp
// Date 2019-11-02

#pragma once

#include <asio.hpp>

#include <algorithm>
#include <tuple>

namespace Libp2p {
    using Iterator = asio::buffers_iterator<asio::const_buffer>;
    /** @brief A match condition function for varints */
    std::pair<Iterator, bool> match_varint(Iterator begin, Iterator end) {
        auto it = std::find_if(
            begin, end, [](auto const& byte) { return not(byte & 0x80); });

        bool success = it != end;

        // one past the end if it's valid
        if (it != end and std::distance(begin, ++it) > 9)
            throw std::runtime_error("varint is too big");

        return std::make_pair(it, success);
    };
} // namespace Libp2p

