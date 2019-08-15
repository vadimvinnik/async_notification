#pragma once

#include <optional>
#include <utility>

namespace async_notification {
  
namespace buffers {

template <typename T>
struct optional {
  using message_t = T;
  using state_t = std::optional<T>;

  static constexpr std::nullopt_t make_empty_state() noexcept {
    return std::nullopt;
  }
  
  static constexpr bool is_empty_state(state_t const& state) noexcept {
    return !state.has_value();
  }

  static_assert(is_empty_state(make_empty_state()));

  template <typename M>
  static void put(state_t& state, M&& message) noexcept {
    state = std::forward<M>(message);
  }

  template <typename F>
  static void handle_and_destroy(F f, state_t& state) {
    f(std::move(*state));
    state.reset();
  }
};

} // namespace storage_traits

} // namespace async_notification

