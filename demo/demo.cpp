#include "manager.h"
#include "buffers/optional.h"
#include "buffers/queue.h"
#include "buffers/simple_variable.h"
#include "buffers/unique_ptr.h"

#include <chrono>
#include <iostream>
#include <thread>

void slow_print_int(int x) {
  using namespace std::chrono_literals;
  
  std::cout << x << std::endl;

  std::this_thread::sleep_for(100ms);
}

template <typename Traits>
void demo_case(std::string const& name) {
  using namespace std::chrono_literals;

  std::cout << name << std::endl;

  auto m = async_notification::manager_factory<Traits>::create(slow_print_int);

  m.post(1);
  std::this_thread::sleep_for(10ms);

  m.post(2);
  m.post(3);
  std::this_thread::sleep_for(10ms);

  m.post(4);
  m.post(5);

  std::this_thread::sleep_for(2s);
}

int main() {
  using simple_int
    = async_notification::buffers::simple_variable<int, 0>;
  using optional_int
    = async_notification::buffers::optional<int>;
  using unique_ptr_int
    = async_notification::buffers::unique_ptr<int>;
  using queue_int
    = async_notification::buffers::queue<int>;

  demo_case<simple_int>("simple");
  demo_case<optional_int>("optional");
  demo_case<unique_ptr_int>("unique_ptr");
  demo_case<queue_int>("queue");

  return 0;
}

