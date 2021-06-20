// Concurrent stack using Folly's hazard pointers

#include <atomic>
#include <iostream>
#include <optional>

#include <folly/synchronization/Rcu.h>

using std::atomic;
using std::optional;
using std::nullopt;
using folly::rcu_reader;
using folly::rcu_obj_base;

template<typename T>
struct stack {
  struct Node : public rcu_obj_base<Node> {
    T t; Node* next;  };

  atomic<Node*> head;

  void push_front(T t) {
    auto p = new Node{{}, t, head.load()};
    while (!head.compare_exchange_weak(p->next, p)) {}
  }

  optional<T> pop_front() {
    rcu_reader guard;
    auto p = head.load();
    while (p && !head.compare_exchange_weak(p, p->next)) { }
    auto val = p ? optional{p->t} : nullopt;
    if (p) p->retire();
    return val;
  }
};

const int M = 10000;

void test_seq() {
  stack<int> s;
  assert(!s.pop_front());
  s.push_front(5);
  assert(s.pop_front().value() == 5);
  assert(!s.pop_front());
  s.push_front(5);
  s.push_front(6);
  s.push_front(7);
  assert(s.pop_front().value() == 7);
  assert(s.pop_front().value() == 6);
  assert(s.pop_front().value() == 5);
}

void test_par() {
  volatile long long checksum1 = 0;
  volatile long long checksum2 = 0;
  volatile long long actualsum1 = 0;
  volatile long long actualsum2 = 0;

  stack<int> s;
  std::atomic<bool> done1 = false;
  std::atomic<bool> done2 = false;

  std::thread pusher1 ([&] () {
    for(int i = 0; i < M; i++) {
      s.push_front(i);
      actualsum1 += i;
    }
    done1 = true;
  });

  std::thread pusher2 ([&] () {
    for(int i = 0; i < M; i++) {
      s.push_front(i);
      actualsum2 += i;
    }
    done2 = true;
  });

  std::thread popper1 ([&] () {
    while(!done1 || !done2) {
      auto val = s.pop_front();
      if(val) checksum1 += val.value();
    }
  });

  std::thread popper2 ([&] () {
    while(!done1 || !done2) {
      auto val = s.pop_front();
      if(val) checksum2 += val.value();
    }
  });

  pusher1.join();
  pusher2.join();

  popper1.join();
  popper2.join();

  assert(checksum1 + checksum2 == actualsum1 + actualsum2);
}

void run_all_tests() {
  test_seq();
  test_par();
}

int main() {
  stack<int> s;
  s.push_front(5);
  auto val = s.pop_front();
  assert(val.has_value());
  assert(val.value() == 5);

  run_all_tests();
}
