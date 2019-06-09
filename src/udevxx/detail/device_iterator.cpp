#include <udevxx/detail/device_iterator.hpp>

namespace udevxx::detail
{
  device_iterator::device_iterator(ref_ptr<udev> context, ref_ptr<udev_enumerate> enumerator, udev_list_entry * entry)
      : m_context{std::move(context)}
      , m_enumerator{std::move(enumerator)}
      , m_entry{entry}
  {
    if (m_entry)
    {
      m_device = device{m_context, udev_list_entry_get_name(m_entry)};
    }
  }

  device_iterator & device_iterator::operator++()
  {
    if ((m_entry = udev_list_entry_get_next(m_entry)))
    {
      m_device = device{m_context, udev_list_entry_get_name(m_entry)};
    }
    else
    {
      m_device.reset();
    }
    return *this;
  }

  device_iterator device_iterator::operator++(int)
  {
    auto old = *this;
    ++(*this);
    return old;
  }

  device const & device_iterator::operator*() const
  {
    return m_device.value();
  }

  device & device_iterator::operator*()
  {
    return m_device.value();
  }

  device const * device_iterator::operator->() const
  {
    return &m_device.value();
  }

  device * device_iterator::operator->()
  {
    return &m_device.value();
  }

  bool device_iterator::operator==(device_iterator const & other) const noexcept
  {
    return (m_context == other.m_context && m_enumerator == other.m_enumerator && m_entry == other.m_entry);
  }

  bool device_iterator::operator!=(device_iterator const & other) const noexcept
  {
    return !(*this == other);
  }

}  // namespace udevxx::detail
