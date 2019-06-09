#ifndef UDEVXX_ENUMERATOR_HPP
#define UDEVXX_ENUMERATOR_HPP

#include <udevxx/detail/ref_ptr.hpp>

#include <libudev.h>

namespace udevxx
{

  struct enumerator
  {
    friend struct context;

    private:
    explicit enumerator(detail::ref_ptr<udev> context);

    detail::ref_ptr<udev> const m_context;
    detail::ref_ptr<udev_enumerate> const m_impl;
  };

}  // namespace udevxx

#endif