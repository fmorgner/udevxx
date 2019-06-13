#ifndef UDEVXX_DETAIL_THREAD_AWARE_HPP
#define UDEVXX_DETAIL_THREAD_AWARE_HPP

#include <stdexcept>
#include <thread>

namespace udevxx::detail
{
  struct thread_aware
  {

    thread_aware() noexcept
        : m_thread_id{std::this_thread::get_id()}
    {
    }

    thread_aware(thread_aware const & other) noexcept
        : m_thread_id{other.m_thread_id}
    {
      check_thread();
    }

    thread_aware(thread_aware && other) noexcept
        : m_thread_id{other.m_thread_id}
    {
      check_thread();
    }

    ~thread_aware()
    {
      check_thread();
    }

    thread_aware & operator=(thread_aware const & other) noexcept
    {
      check_thread();
      return *this;
    }

    thread_aware & operator=(thread_aware && other) noexcept
    {
      check_thread();
      return *this;
    }

    void check_thread() const
    {
      if (m_thread_id != std::this_thread::get_id())
      {
        throw std::logic_error{"libudev objects may not be shared across threads, even if all access is synchronized "
                               "(see man 3 libudev)!"};
      }
    }

    private:
    std::thread::id m_thread_id;
  };
}  // namespace udevxx::detail

#endif