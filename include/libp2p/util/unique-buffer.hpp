/** Unique Buffer
 *
 * @author Matthew Knight
 * @filename unique-buffer.hpp
 * @date 2019-11-02
 *
 * This buffer is useful for constructing bytes to send over the wire with asio;
 */

#pragma once

#include <asio.hpp>

#include <memory>
#include <vector>

namespace Libp2p {
    class UniqueBuffer {
        using Storage = std::vector<std::uint8_t>;
        std::unique_ptr<Storage> input;
        std::unique_ptr<Storage> output;

      public:
        using const_buffers_type = asio::const_buffer;
        using mutable_buffers_type = asio::mutable_buffer;

        UniqueBuffer() = default;

        template <typename Vector,
                  typename = std::enable_if_t<
                      not std::is_same_v<std::decay_t<Vector>, UniqueBuffer>>>
        UniqueBuffer(Vector&& vector)
            : input(std::make_unique<Storage>(vector)) {}

        template <typename Iterator>
        UniqueBuffer(Iterator begin, Iterator end)
            : input(std::make_unique<Storage>(begin, end)) {}

        std::size_t size() { return input->size(); }

        std::size_t max_size() {
            return input->max_size() + output->max_size();
        }

        std::size_t capacity() {
            return input->capacity() + output->capacity();
        }

        const_buffers_type data() { return asio::buffer(*input); }

        mutable_buffers_type prepare(std::size_t n) {
            return asio::buffer(*output);
        }

        void commit(std::size_t n) {}

        void consume(std::size_t n) {}

        auto begin() { return input->begin(); }

        auto end() { return input->end(); }
    };
} // namespace Libp2p
