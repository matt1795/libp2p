// Transport Information
//
// Author: Matthew Knight
// File Name: transports.hpp
// Date: 2019-11-02

#pragma once

#include <asio.hpp>

template <typename Protocol>
struct ProtocolInfo;

template <>
struct ProtocolInfo<asio::ip::tcp> {
    static constexpr bool secure = false;
    static constexpr bool multiplexed = false;
};
