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

}  // namespace udevxx
