#include <udevxx/device.hpp>
#include <udevxx/device_enumerator.hpp>

#include <utility>

namespace udevxx
{

  device_enumerator::device_enumerator(detail::ref_ptr<udev> context)
      : m_context{std::move(context)}
      , m_impl{m_context.get()}
  {
  }

  device_enumerator::iterator device_enumerator::begin() const noexcept
  {
    udev_enumerate_scan_devices(m_impl.get());
    return iterator{m_context, m_impl, udev_enumerate_get_list_entry(m_impl.get())};
  }

  device_enumerator::iterator device_enumerator::end() const noexcept
  {
    return iterator{m_context, m_impl, nullptr};
  }

  device_enumerator & device_enumerator::match(subsystem subsystem)
  {
    udev_enumerate_add_match_subsystem(m_impl.get(), subsystem->c_str());
    return *this;
  }

  device_enumerator & device_enumerator::dont_match(subsystem subsystem)
  {
    udev_enumerate_add_nomatch_subsystem(m_impl.get(), subsystem->c_str());
    return *this;
  }

  device_enumerator & device_enumerator::match(initialized_t)
  {
    udev_enumerate_add_match_is_initialized(m_impl.get());
    return *this;
  }

}  // namespace udevxx
