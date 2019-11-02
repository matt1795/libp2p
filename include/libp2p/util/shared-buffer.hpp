/** Shared Buffer
 *
 * @author Matthew Knight
 * @filename shared-buffer.hpp
 * @date 2019-11-02
 *
 * This buffer is useful as a receiving buffer with asio.
 */

#pragma once

#include <asio.hpp>

#include <memory>
#include <vector>

namespace Libp2p {
    class SharedBuffer {
        using Storage = std::vector<std::uint8_t>;
        std::shared_ptr<Storage> buf;

      public:
        using const_buffers_type = asio::const_buffer;
        using mutable_buffers_type = asio::mutable_buffer;

        SharedBuffer()
            : buf(std::make_shared<Storage>()) {}

        std::size_t size() const { return buf->size(); }

        std::size_t max_size() const { return buf->max_size(); }

        std::size_t capacity() const { return buf->capacity(); }

        const_buffers_type data(std::size_t pos, std::size_t n) const {
            return asio::buffer(*buf);
        }

        mutable_buffers_type data(std::size_t pos, std::size_t n) {
            return asio::mutable_buffer{buf->data(), buf->size()};
        }

        void grow(std::size_t n) { buf->resize(buf->size() + n); }

        void shrink(std::size_t n) {
            if (buf->size() < n)
                buf->empty();
            else
                buf->resize(buf->size() - n);
        }

        void consume(std::size_t n) {
            if (buf->size() < n)
                buf->empty();
            else
                buf->erase(buf->begin(), std::next(buf->begin(), n));
        }

        auto begin() { return buf->begin(); }

        auto end() { return buf->end(); }
    };
} // namespace Libp2p
