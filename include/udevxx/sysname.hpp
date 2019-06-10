#ifndef UDEVXX_SYSNAME_HPP
#define UDEVXX_SYSNAME_HPP

#include <udevxx/detail/strong_type.hpp>

#include <iosfwd>
#include <string>

namespace udevxx
{

  using sysname = detail::strong_type<std::string, struct sysname_tag>;

  std::ostream & operator<<(std::ostream & out, sysname value);

  namespace literals
  {
    sysname operator""_name(char const * string, unsigned long length);
  }

}  // namespace udevxx

#endif