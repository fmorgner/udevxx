#ifndef UDEVXX_DEVICE_KIND_HPP
#define UDEVXX_DEVICE_KIND_HPP

namespace udevxx
{
  enum struct device_kind : char
  {
    block = 'b',
    character = 'c',
  };
}

#endif