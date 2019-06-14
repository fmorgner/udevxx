#ifndef UDEVXX_CONTEXT_HPP
#define UDEVXX_CONTEXT_HPP

#include <udevxx/detail/raw_type_owner.hpp>
#include <udevxx/detail/thread_aware.hpp>
#include <udevxx/device_enumerator.hpp>

#include <libudev.h>

namespace udevxx
{
  struct context
      : detail::raw_type_owner<udev>
      , detail::thread_aware
  {
    context()
        : detail::raw_type_owner<udev>(udev_new(), udev_ref, udev_unref)
    {
    }

    device_enumerator devices() const
    {
      check_thread();
      return device_enumerator(*this);
    }

    std::vector<subsystem> subsystems() const
    {
      check_thread();
      auto enumerator =
          detail::raw_type_owner<udev_enumerate>{udev_enumerate_new(m_raw), udev_enumerate_ref, udev_enumerate_unref};
      udev_enumerate_scan_subsystems(enumerator.get());
      auto subsystem_list = detail::list<subsystem, std::string>{udev_enumerate_get_list_entry(enumerator.get())};
      return {subsystem_list.begin(), subsystem_list.end()};
    }
  };
}  // namespace udevxx

#endif