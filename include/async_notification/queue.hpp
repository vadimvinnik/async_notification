#pragma once

#include <queue>
#include <utility>

namespace async_notification {
  
namespace buffers {

template <typename T>
struct queue {
  using message_t = T;
  using state_t = std::queue<T>;

  static state_t make_empty_state() noexcept {
    return {};
  }
  
  static bool is_empty_state(state_t const& state) noexcept {
    return state.empty();
  }

  template <typename M>
  static void put(state_t& state, M&& message) noexcept {
    state.push(std::forward<M>(message));
  }

  template <typename F>
  static void handle_and_destroy(F f, state_t& state) {
    while (!state.empty())
    {
        f(std::move(state.front()));
        state.pop();
    }
  }
};

} // namespace storage_traits

} // namespace async_notification

