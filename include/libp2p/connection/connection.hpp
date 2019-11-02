/** Secure, Multiplexed Connection
 *
 * Reference: https://github.com/libp2p/interface-connection
 * @author Matthew Knight
 * @filename connection.hpp
 * @date 2019-09-12
 */

#pragma once

#include "libp2p/connection/transports.hpp"
#include "libp2p/util/matchers.hpp"
#include "libp2p/util/shared-buffer.hpp"
#include "libp2p/util/unique-buffer.hpp"

#include <asio.hpp>
#include <multiformats/multistream-select.hpp>
#include <multiformats/varint.hpp>

#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>

namespace Libp2p {
    template <typename Transport, typename SecurityLayer,
              typename MultiplexLayer>
    class Connection
        : public std::enable_shared_from_this<
              Connection<Transport, SecurityLayer, MultiplexLayer>> {

        Transport transport;
        SharedBuffer rxbuf;
        typename SecurityLayer::State security;
        typename MultiplexLayer::State multiplexer;

        template <typename Protocol>
        auto protocol_message() {
            return Multiformats::MultistreamSelect::protocol_message<
                Protocol>();
        }

        auto protocol_message(std::string const& protocol) {
            std::vector<std::uint8_t> ret;
            Multiformats::Varint len{protocol.size() + 1};
            ret.reserve(len.size() + protocol.size() +
                        Multiformats::MultistreamSelect::delimiter.size());
            auto inserter = std::back_inserter(ret);

            std::copy(len.begin(), len.end(), inserter);
            std::copy(protocol.cbegin(), protocol.end(), inserter);
            std::copy(Multiformats::MultistreamSelect::delimiter.cbegin(),
                      Multiformats::MultistreamSelect::delimiter.cend(),
                      inserter);

            return ret;
        }

        template <typename MatchCondition>
        std::size_t read_until(MatchCondition match_condition) {
            return asio::read_until(transport, rxbuf, match_condition);
        }

        template <typename CompletionCondition>
        std::size_t read(CompletionCondition completion_condition) {
            return asio::read(transport, rxbuf, completion_condition);
        }

        std::size_t read_varint_message() {
            std::size_t n = read_until(match_varint);
            Multiformats::Varint len{rxbuf.begin(), rxbuf.end()};
            rxbuf.consume(len.size());

            if (len > rxbuf.size())
                read(asio::transfer_exactly(len - rxbuf.size()));

            return rxbuf.size();
        }

        template <typename Protocol>
        bool initiator_negotiate_protocol() {
            asio::write(transport, UniqueBuffer{protocol_message<Protocol>()});
            read_varint_message();

            auto it = std::find(rxbuf.begin(), rxbuf.end(), '\n');
            if (it == rxbuf.end())
                throw std::runtime_error("protocol parsing error");

            std::string protocol{rxbuf.begin(), it};
            rxbuf.consume(protocol.size() + 1);
            if (protocol == Protocol::name)
                return true;
            else if (protocol == Multiformats::MultistreamSelect::na)
                return false;
            else
                throw std::runtime_error("invalid protocol response");
        }

        template <typename Protocol, typename... Protocols>
        bool initiator_try_protocols() {
            if (initiator_negotiate_protocol<Protocol>())
                return true;

            if constexpr (sizeof...(Protocols) > 0)
                return initiator_try_protocols<Protocols...>();

            return false;
        }

        template <typename... Protocols>
        bool initiator_negotiate_protocols(std::tuple<Protocols...>&&) {
            return initiator_try_protocols<Protocols...>();
        }

        template <typename... Protocols>
        void responder_negotiate_protocols(std::tuple<Protocols...>&&) {
            std::vector<std::string_view> protocol_names{Protocols::name...};

            std::string request;
            bool have_protocol = false;

            do {
                read_varint_message();

                auto it = std::find(rxbuf.begin(), rxbuf.end(), '\n');
                if (it == rxbuf.end())
                    throw std::runtime_error("protocol parsing error");

                request = std::string{rxbuf.begin(), it};
                rxbuf.consume(request.size() + 1);

                have_protocol =
                    std::any_of(protocol_names.cbegin(), protocol_names.cend(),
                                [&request](auto const& protocol) {
                                    return protocol == request;
                                });
            } while (!have_protocol);

            asio::write(transport, UniqueBuffer{protocol_message(request)});
        }

        template <typename Protocol>
        void responder_negotiate_protocol() {
            responder_negotiate_protocols(std::tuple<Protocol>{});
        }

      public:
        /** @brief Initiate connection to another host */
        template <typename Executor>
        Connection(Executor& executor, std::string const& host,
                   std::string const& port)
            : transport(executor) {
            asio::ip::tcp::resolver resolver{executor};
            auto endpoints = resolver.resolve(host, port);

            asio::connect(transport, endpoints);
            if (not initiator_negotiate_protocol<
                    Multiformats::MultistreamSelect>())
                throw std::runtime_error("multistream-select failure");
            if (not initiator_negotiate_protocols(
                    typename SecurityLayer::Protocols{}))
                throw std::runtime_error("security negotiation failure");
            if (not initiator_negotiate_protocols(
                    typename MultiplexLayer::Protocols{}))
                throw std::runtime_error("multiplexer negotiation error");
        }

        /** @brief accept connection from another host */
        Connection(Transport&& transport)
            : transport(std::forward<Transport>(transport)) {
            responder_negotiate_protocol<Multiformats::MultistreamSelect>();
            responder_negotiate_protocols(typename SecurityLayer::Protocols{});
            responder_negotiate_protocols(typename MultiplexLayer::Protocols{});
        }
    };
} // namespace Libp2p
