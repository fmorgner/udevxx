#ifndef UDEVXX_SUBSYSTEM_HPP
#define UDEVXX_SUBSYSTEM_HPP

#include <udevxx/detail/strong_type.hpp>

#include <ostream>
#include <string>

namespace udevxx
{

  using subsystem = detail::strong_type<std::string, struct subsystem_tag>;

  std::ostream & operator<<(std::ostream & output, subsystem const & value);

  namespace literals
  {

    subsystem operator""_sub(char const * string, unsigned long length);

  }  // namespace literals

}  // namespace udevxx

#endif