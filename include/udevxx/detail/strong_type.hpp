#ifndef UDEVXX_DETAIL_STRONG_TYPE_HPP
#define UDEVXX_DETAIL_STRONG_TYPE_HPP

#include <string>

namespace udevxx::detail
{

  template <typename ValueType, typename Tag>
  struct strong_type
  {
    explicit strong_type(ValueType value)
        : m_value{std::move(value)}
    {
    }

    friend ValueType unsafe_decay_cast(strong_type const & wrapped)
    {
      return wrapped.m_value;
    }

    ValueType const * operator->() const noexcept
    {
      return &m_value;
    }

    ValueType const & operator*() const noexcept
    {
      return m_value;
    }

    private:
    ValueType m_value;
  };

}  // namespace udevxx::detail

#endif