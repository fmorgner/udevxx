#ifndef UDEVXX_TAGGED_TYPES_HPP
#define UDEVXX_TAGGED_TYPES_HPP

#include <libudev.h>

#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

namespace udevxx
{
  struct tagged_type_tag
  {
  };

  template <typename UnderlyingType, typename Tag>
  struct tagged_type : tagged_type_tag
  {
    using underlying_type = UnderlyingType;

    explicit tagged_type(UnderlyingType value) noexcept(std::is_nothrow_move_constructible_v<UnderlyingType>)
        : m_real{std::move(value)}
    {
    }

    underlying_type * operator->() noexcept
    {
      return &m_real;
    }

    underlying_type const * operator->() const noexcept
    {
      return &m_real;
    }

    underlying_type & get() noexcept
    {
      return m_real;
    }

    underlying_type const & get() const noexcept
    {
      return m_real;
    }

    private:
    underlying_type m_real;
  };

  using subsystem = tagged_type<std::string, struct subsystem_tag>;
  using system_path = tagged_type<std::string, struct system_path_tag>;
  using device_number = tagged_type<dev_t, struct device_number_tag>;

  template <typename TaggedType, typename = std::enable_if_t<std::is_base_of_v<tagged_type_tag, TaggedType>>>
  std::ostream & operator<<(std::ostream & out, TaggedType const & tagged)
  {
    return out << tagged.get();
  }

  namespace literals
  {
    subsystem operator""_sub(char const * string, unsigned long length)
    {
      return subsystem{{string, length}};
    }

    system_path operator""_path(char const * string, unsigned long length)
    {
      return system_path{{string, length}};
    }
  }  // namespace literals

}  // namespace udevxx

#endif