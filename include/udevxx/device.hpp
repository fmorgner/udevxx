#ifndef UDEVXX_DEVICE_HPP
#define UDEVXX_DEVICE_HPP

#include <udevxx/detail/ref_ptr.hpp>
#include <udevxx/subsystem.hpp>
#include <udevxx/sysname.hpp>
#include <udevxx/syspath.hpp>

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

    udevxx::subsystem subsystem() const;
    udevxx::syspath system_path() const;
    udevxx::sysname system_name() const;

    private:
    device(detail::ref_ptr<udev> context, syspath path);

    detail::ref_ptr<udev> m_context;
    detail::ref_ptr<udev_device> m_impl;
  };

}  // namespace udevxx

#endif