#include <utility>

namespace async_notification {
  
namespace buffers {

template <typename T, T Empty>
struct simple_variable {
  using message_t = T;
  using state_t = T;

  static constexpr state_t make_empty_state() noexcept {
    return Empty;
  }
  
  static constexpr bool is_empty_state(state_t const& state) noexcept {
    return state == Empty;
  }

  static_assert(is_empty_state(make_empty_state()));

  template <typename M>
  static void put(state_t& state, M&& message) noexcept {
    state = std::forward<M>(message);
  }

  template <typename F>
  static void handle_and_destroy(F f, state_t& state) {
    f(std::move(state));
    state = Empty;
  }
};

} // namespace storage_traits

} // namespace async_notification

