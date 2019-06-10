#ifndef UDEVXX_ENUMERATOR_HPP
#define UDEVXX_ENUMERATOR_HPP

#include <udevxx/detail/device_iterator.hpp>
#include <udevxx/detail/ref_ptr.hpp>
#include <udevxx/device.hpp>

#include <libudev.h>

#include <iterator>
#include <utility>

namespace udevxx
{

  constexpr struct initialized_tag
  {
    constexpr explicit initialized_tag(bool = true) noexcept
    {
    }
  } initialized;

  struct enumerator
  {
    friend struct context;

    using iterator = detail::device_iterator;

    iterator begin() const noexcept;
    iterator end() const noexcept;

    enumerator & match(subsystem subsystem);
    enumerator & dont_match(subsystem subsystem);

    enumerator & match(initialized_tag initialized);

    private:
    explicit enumerator(detail::ref_ptr<udev> context);

    detail::ref_ptr<udev> const m_context;
    detail::ref_ptr<udev_enumerate> const m_impl;
  };

}  // namespace udevxx

#endif