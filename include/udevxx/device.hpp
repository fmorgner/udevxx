#ifndef UDEVXX_DEVICE_HPP
#define UDEVXX_DEVICE_HPP

#include <udevxx/detail/raw_type_owner.hpp>
#include <udevxx/tagged_types.hpp>

#include <libudev.h>

namespace udevxx
{
  struct device : detail::raw_type_owner<udev_device>
  {
    using context_type = detail::raw_type_owner<udev>;

    device(context_type const & context, system_path const & path)
        : device{udev_device_new_from_syspath(context.get(), path->c_str())}
    {
    }

    udevxx::system_path system_path() const
    {
      return udevxx::system_path{{udev_device_get_syspath(m_raw)}};
    }

    udevxx::subsystem subsystem() const
    {
      return udevxx::subsystem{{udev_device_get_subsystem(m_raw)}};
    }

    private:
    device(udev_device * raw)
        : detail::raw_type_owner<udev_device>(raw, udev_device_ref, udev_device_unref)
    {
    }
  };  // namespace udevxx
}  // namespace udevxx

#endif