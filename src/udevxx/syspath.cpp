#include <udevxx/syspath.hpp>

namespace udevxx
{

  std::ostream & operator<<(std::ostream & output, syspath const & value)
  {
    return output << "SYSPATH: " << unsafe_decay_cast(value);
  }

  namespace literals
  {

    syspath operator""_syspath(char const * string, unsigned long size)
    {
      return syspath{{string, size}};
    }

  }  // namespace literals

}  // namespace udevxx
