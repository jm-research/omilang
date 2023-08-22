#ifndef OMI_SUPPORT_IMMER_LOCK_SPINLOCKPOLICY_HPP
#define OMI_SUPPORT_IMMER_LOCK_SPINLOCKPOLICY_HPP

#include <atomic>
#include <thread>

#if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
#define IMMER_SMT_PAUSE __asm__ __volatile__("rep; nop" : : : "memory")
#endif

namespace omi::support::immer {

struct spinlock_policy {
  std::atomic_flag v_{};

  bool try_lock() { return !v_.test_and_set(std::memory_order_acquire); }

  void lock() {
    for (auto k = 0u; !try_lock(); ++k) {
      if (k < 4)
        continue;
#ifdef IMMER_SMT_PAUSE
      else if (k < 16)
        IMMER_SMT_PAUSE;
#endif
      else
        std::this_thread::yield();
    }
  }

  void unlock() { v_.clear(std::memory_order_release); }

  struct scoped_lock {
    scoped_lock(const scoped_lock&) = delete;
    scoped_lock& operator=(const scoped_lock&) = delete;

    explicit scoped_lock(spinlock_policy& sp) : sp_{sp} { sp.lock(); }

    ~scoped_lock() { sp_.unlock(); }

   private:
    spinlock_policy& sp_;
  };
};

}  // namespace omi::support::immer

#endif  // OMI_SUPPORT_IMMER_LOCK_SPINLOCKPOLICY_HPP