#ifndef UDEVXX_DETAIL_LIST_HPP
#define UDEVXX_DETAIL_LIST_HPP

#include <libudev.h>

#include <cstddef>
#include <iterator>
#include <optional>

namespace udevxx::detail
{

  template <typename NameType, typename ValueType>
  struct list
  {
    struct entry
    {
      constexpr entry(udev_list_entry * raw) noexcept
          : m_raw{raw}
      {
      }

      NameType name() const noexcept(noexcept(NameType{std::declval<char const *>()}))
      {
        auto raw_name = udev_list_entry_get_name(m_raw);
        return NameType{raw_name ? raw_name : ""};
      }

      ValueType value() const noexcept(noexcept(ValueType{std::declval<char const *>()}))
      {
        auto raw_value = udev_list_entry_get_value(m_raw);
        return ValueType{raw_value ? raw_value : ""};
      }

      operator ValueType() const noexcept(noexcept(value()))
      {
        return value();
      }

      operator NameType() const noexcept(noexcept(name()))
      {
        return name();
      }

      entry & operator++() noexcept
      {
        m_raw = udev_list_entry_get_next(m_raw);
        return *this;
      }

      constexpr bool operator==(entry const & other) const noexcept
      {
        return m_raw == other.m_raw;
      }

      constexpr operator bool() const noexcept
      {
        return m_raw;
      }

      private:
      udev_list_entry * m_raw;
    };

    struct iterator
    {
      using iterator_category = std::forward_iterator_tag;
      using difference_type = std::ptrdiff_t;
      using value_type = entry const;
      using reference = value_type &;
      using pointer = value_type *;

      explicit iterator(udev_list_entry * entry)
          : m_entry{entry}
      {
      }

      iterator & operator++() noexcept
      {
        ++m_entry;
        return *this;
      }

      iterator operator++(int) noexcept
      {
        auto copy = m_entry;
        ++*this;
        return copy;
      }

      constexpr bool operator==(iterator const & other) const noexcept
      {
        return m_entry == other.m_entry;
      }

      constexpr bool operator!=(iterator const & other) const noexcept
      {
        return !(*this == other);
      }

      entry operator*() const noexcept
      {
        return m_entry;
      }

      private:
      entry m_entry;
    };

    explicit list(udev_list_entry * head)
        : m_head{head}
    {
    }

    constexpr iterator begin() const noexcept
    {
      return iterator{m_head};
    }

    constexpr iterator end() const noexcept
    {
      return iterator{nullptr};
    }

    private:
    udev_list_entry * m_head;
  };

}  // namespace udevxx::detail

#endif