#ifndef UDEVXX_SYSPATH_HPP
#define UDEVXX_SYSPATH_HPP

#include <udevxx/detail/strong_type.hpp>

#include <ostream>
#include <string>

namespace udevxx
{

  using syspath = detail::strong_type<std::string, struct syspath_tag>;

  inline syspath operator""_syspath(char const * string, unsigned long size)
  {
    return syspath{{string, size}};
  }

  inline std::ostream & operator<<(std::ostream & output, syspath const & value)
  {
    return output << "SYSPATH: " << unsafe_decay_cast(value);
  }

}  // namespace udevxx

#endif