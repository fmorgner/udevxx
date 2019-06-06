#ifndef UDEVXX_UDEV_CONTEXT_HPP
#define UDEVXX_UDEV_CONTEXT_HPP

#include <udevxx/detail/ref_ptr.hpp>

#include <libudev.h>

namespace udevxx
{

  struct context
  {
    void swap(context & other) noexcept;

    private:
    detail::ref_ptr<udev> m_impl;
  };

  void swap(context & lhs, context & rhs) noexcept;

};  // namespace udevxx

#endif