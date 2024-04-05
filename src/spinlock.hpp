#pragma once

#include <atomic>

namespace aptos
{

class [[nodiscard]] spinlock
{
public:
    using lockable_type = std::atomic_flag;

    [[nodiscard]] constexpr spinlock() noexcept = default;

    spinlock(const spinlock&) noexcept            = delete;
    spinlock& operator=(const spinlock&) noexcept = delete;

    void lock() noexcept
    {
        while (lockable_.test_and_set(std::memory_order_acquire))
        {
            while (lockable_.test(std::memory_order_relaxed))
            {
            }
        }
    }

    bool try_lock() noexcept
    {
        return lockable_.test_and_set(std::memory_order_acquire);
    }

    void unlock() noexcept { lockable_.clear(std::memory_order_release); }

private:
    std::atomic_flag lockable_{};
};

} // namespace aptos
