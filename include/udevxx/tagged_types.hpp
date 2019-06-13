#ifndef UDEVXX_TAGGED_TYPES_HPP
#define UDEVXX_TAGGED_TYPES_HPP

#include <libudev.h>

#include <string>
#include <type_traits>
#include <utility>

namespace udevxx
{
  template <typename UnderlyingType, typename Tag>
  struct tagged_type
  {
    explicit tagged_type(UnderlyingType value) noexcept(std::is_nothrow_move_constructible_v<UnderlyingType>)
        : m_real{std::move(value)}
    {
    }

    UnderlyingType * operator->() noexcept
    {
      return &m_real;
    }

    UnderlyingType const * operator->() const noexcept
    {
      return &m_real;
    }

    private:
    UnderlyingType m_real;
  };

  using subsystem = tagged_type<std::string, struct subsystem_tag>;
  using system_path = tagged_type<std::string, struct system_path_tag>;
  using device_number = tagged_type<dev_t, struct device_number_tag>;

  namespace literals
  {
    subsystem operator""_sub(char const * string, unsigned long length)
    {
      return subsystem{{string, length}};
    }

    system_path operator""_sys(char const * string, unsigned long length)
    {
      return system_path{{string, length}};
    }
  }  // namespace literals

}  // namespace udevxx

#endif