#pragma once

#include <cassert>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace async_notification {
  
template <typename Buffer, typename Handler>
class manager {
  using state_t = typename Buffer::state_t;
  using this_t = manager<Buffer, Handler>;

  Handler handler_;
  state_t state_;
  std::mutex mx_;
  std::condition_variable cv_;
  bool is_stopping_;
  std::thread thread_;

  bool can_proceed() const noexcept
  {
    return is_stopping_ || !Buffer::is_empty_state(state_);
  }

  void run() {
    while (true) {
      state_t state { Buffer::make_empty_state() };

      {
        std::unique_lock<std::mutex> lock { mx_ };
        cv_.wait(
          lock,
          [this]() { return can_proceed(); });

        if (is_stopping_)
          return;

        std::swap(state_, state);
      }

      assert(!Buffer::is_empty_state(state));
      Buffer::handle_and_destroy(handler_, state);
      assert(Buffer::is_empty_state(state));
    }
  }

public: 
  explicit manager(Handler handler)
    : handler_ { handler }
    , state_ { Buffer::make_empty_state() }
    , is_stopping_ { false }
    , thread_ { std::thread(&this_t::run, this) }
  {}

  ~manager() {
    is_stopping_ = true;
    cv_.notify_one();
    thread_.join();
  }

  template <typename M>
  void post(M&& message) {
    {
      std::lock_guard<std::mutex> lock { mx_ };
      Buffer::put(state_, std::forward<M>(message));
    }

    cv_.notify_one();
  }
};

template <typename Buffer>
struct manager_factory {
  template <typename Handler>
  static manager<Buffer, Handler> create(Handler handler) {
    return manager<Buffer, Handler>{ handler };
  }
};

} // namespace async_notification

