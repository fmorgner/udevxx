#ifndef UDEVXX_SUBSYSTEM_HPP
#define UDEVXX_SUBSYSTEM_HPP

#include <udevxx/detail/strong_type.hpp>

#include <ostream>
#include <string>

namespace udevxx
{

  using subsystem = detail::strong_type<std::string, struct subsystem_tag>;

  inline std::ostream & operator<<(std::ostream & output, subsystem const & value)
  {
    return output << "SUBSYSTEM: " << unsafe_decay_cast(value);
  }

}  // namespace udevxx

#endif