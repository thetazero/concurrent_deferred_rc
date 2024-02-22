#include "gtest/gtest.h"

#include <cdrc/atomic_weak_ptr.h>
#include <cdrc/atomic_rc_ptr.h>
#include <cdrc/rc_ptr.h>
#include <cdrc/weak_ptr.h>

TEST(MiscPtr, AtomicRcStore) {
  cdrc::atomic_rc_ptr<int> ap;
  cdrc::rc_ptr<int> x = cdrc::make_rc<int>(0);
  ap.store(x);
  ASSERT_EQ(x.use_count(), 2);
  ASSERT_EQ(x.weak_count(), 0);

  ap.store(nullptr);
  ASSERT_EQ(x.use_count(), 2);
  ASSERT_EQ(x.weak_count(), 0);
}

TEST(MiscPtr, RcCopy) {
  cdrc::rc_ptr<int> x = cdrc::make_rc<int>(0);
  cdrc::rc_ptr<int> &z = x;
  cdrc::rc_ptr<int> y = z;
  ASSERT_EQ(x.use_count(), 2);
  ASSERT_EQ(y.use_count(), 2);
  ASSERT_EQ(x.weak_count(), 0);
  ASSERT_EQ(y.weak_count(), 0);
}

TEST(MiscPtr, Random) {
  cdrc::rc_ptr<int> x = cdrc::make_rc<int>(0);
  cdrc::weak_ptr<int> y = x;
  cdrc::rc_ptr<int> z = y.lock();
  z = nullptr;
  cdrc::atomic_rc_ptr<int> ap = z;
  ap = y;
  cdrc::rc_ptr<int> w = ap.load();
}
