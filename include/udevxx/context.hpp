#ifndef UDEVXX_CONTEXT_HPP
#define UDEVXX_CONTEXT_HPP

#include <udevxx/detail/raw_type_owner.hpp>

#include <libudev.h>

namespace udevxx
{
  struct context : detail::raw_type_owner<udev>
  {
    context()
        : detail::raw_type_owner<udev>(udev_new(), udev_ref, udev_unref)
    {
    }
  };
}  // namespace udevxx

#endif