#ifndef UDEVXX_DETAIL_DEVICE_ITERATOR_HPP
#define UDEVXX_DETAIL_DEVICE_ITERATOR_HPP

#include <udevxx/device.hpp>

#include <libudev.h>

#include <iterator>
#include <optional>
#include <stdexcept>
#include <utility>

namespace udevxx::detail
{
  struct device_iterator
  {
    using iterator_category = std::forward_iterator_tag;
    using value_type = device;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type *;
    using reference = value_type *;

    device_iterator(ref_ptr<udev> context, ref_ptr<udev_enumerate> enumerator, udev_list_entry * entry);

    device_iterator & operator++();
    device_iterator operator++(int);

    device const & operator*() const;
    device & operator*();

    device const * operator->() const;
    device * operator->();

    bool operator==(device_iterator const & other) const noexcept;
    bool operator!=(device_iterator const & other) const noexcept;

    private:
    ref_ptr<udev> const m_context;
    ref_ptr<udev_enumerate> const m_enumerator;
    udev_list_entry * m_entry;
    std::optional<value_type> m_device;
  };

}  // namespace udevxx::detail

#endif