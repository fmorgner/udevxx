#include <udevxx/sysname.hpp>

#include <ostream>

namespace udevxx
{

  std::ostream & operator<<(std::ostream & out, sysname value)
  {
    return out << "SYSNAME: " << unsafe_decay_cast(value);
  }

  namespace literals
  {

    sysname operator""_name(char const * string, unsigned long length)
    {
      return sysname{{string, length}};
    }

  }  // namespace literals

}  // namespace udevxx
