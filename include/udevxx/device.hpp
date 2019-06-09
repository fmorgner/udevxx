#ifndef UDEVXX_DEVICE_HPP
#define UDEVXX_DEVICE_HPP

#include <udevxx/detail/ref_ptr.hpp>

#include <string>

namespace udevxx
{

  namespace detail
  {
    struct device_iterator;
  }

  struct device
  {
    friend struct enumerator;
    friend detail::device_iterator;

    std::string subsystem() const;
    std::string system_path() const;

    private:
    device(detail::ref_ptr<udev> context, std::string syspath);

    detail::ref_ptr<udev> m_context;
    detail::ref_ptr<udev_device> m_impl;
  };

}  // namespace udevxx

#endif