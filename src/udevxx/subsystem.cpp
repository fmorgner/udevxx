#include <udevxx/subsystem.hpp>

namespace udevxx
{

  std::ostream & operator<<(std::ostream & output, subsystem const & value)
  {
    return output << "SUBSYSTEM: " << unsafe_decay_cast(value);
  }

  namespace literals
  {

    subsystem operator""_sub(char const * string, unsigned long length)
    {
      return subsystem{{string, length}};
    }

  }  // namespace literals

}  // namespace udevxx