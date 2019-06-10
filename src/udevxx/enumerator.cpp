#include <udevxx/device.hpp>
#include <udevxx/enumerator.hpp>

#include <utility>

namespace udevxx
{

  enumerator::enumerator(detail::ref_ptr<udev> context)
      : m_context{std::move(context)}
      , m_impl{m_context.get()}
  {
  }

  enumerator::iterator enumerator::begin() const noexcept
  {
    udev_enumerate_scan_devices(m_impl.get());
    return iterator{m_context, m_impl, udev_enumerate_get_list_entry(m_impl.get())};
  }

  enumerator::iterator enumerator::end() const noexcept
  {
    return iterator{m_context, m_impl, nullptr};
  }

  enumerator & enumerator::match(subsystem subsystem)
  {
    udev_enumerate_add_match_subsystem(m_impl.get(), subsystem->c_str());
    return *this;
  }

  enumerator & enumerator::dont_match(subsystem subsystem)
  {
    udev_enumerate_add_nomatch_subsystem(m_impl.get(), subsystem->c_str());
    return *this;
  }

  enumerator & enumerator::match(initialized_t)
  {
    udev_enumerate_add_match_is_initialized(m_impl.get());
    return *this;
  }

}  // namespace udevxx
