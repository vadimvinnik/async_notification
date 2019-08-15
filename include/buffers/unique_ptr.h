#pragma once

#include <memory>
#include <utility>

namespace async_notification {
  
namespace buffers {

template <typename T>
struct unique_ptr {
  using message_t = T;
  using state_t = std::unique_ptr<T>;

  static constexpr std::nullptr_t make_empty_state() noexcept {
    return nullptr;
  }
  
  static constexpr bool is_empty_state(state_t const& state) noexcept {
    return nullptr == state;
  }

  template <typename M>
  static void put(state_t& state, M&& message) noexcept {
    state = std::make_unique<message_t>(std::forward<M>(message));
  }

  template <typename F>
  static void handle_and_destroy(F f, state_t& state) {
    f(std::move(*state));
    state.reset();
  }
};

} // namespace storage_traits

} // namespace async_notification

