#ifndef UDEVXX_DEVICE_NUMBER_HPP
#define UDEVXX_DEVICE_NUMBER_HPP

#include <libudev.h>

#include <ostream>
#include <type_traits>

namespace udevxx
{

  struct device_number
  {
    constexpr device_number() noexcept = default;

    constexpr explicit device_number(dev_t number) noexcept
        : m_number{number}
    {
    }

    device_number(unsigned int major, unsigned int minor) noexcept
        : m_number{makedev(major, minor)}
    {
    }

    unsigned int major_number() const noexcept
    {
      return major(m_number);
    }

    unsigned int minor_number() const noexcept
    {
      return minor(m_number);
    }

    private:
    dev_t m_number{};
  };

  std::ostream & operator<<(std::ostream & out, device_number number)
  {
    return out << number.major_number() << ':' << number.minor_number();
  }

}  // namespace udevxx

#endif