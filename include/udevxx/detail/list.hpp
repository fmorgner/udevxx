#ifndef UDEVXX_DETAIL_LIST_HPP
#define UDEVXX_DETAIL_LIST_HPP

#include <udevxx/detail/api_utilities.hpp>

#include <libudev.h>

#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <optional>
#include <utility>

namespace udevxx::detail
{

  template <typename NameType, typename ValueType>
  struct list
  {
    using name_factory_type = std::function<NameType(char const *)>;
    using value_factory_type = std::function<ValueType(char const *)>;

    struct entry : std::pair<NameType, ValueType>
    {
      using base_pair = std::pair<NameType, ValueType>;

      using base_pair::first;
      using base_pair::second;

      entry(udev_list_entry * raw, name_factory_type name_factory, value_factory_type value_factory)
          : base_pair{name_factory(detail::nullable_c_string(udev_list_entry_get_name, raw)),
                      value_factory(detail::nullable_c_string(udev_list_entry_get_value, raw))}
          , m_raw{raw}
      {
      }

      NameType name() const noexcept(noexcept(std::declval<name_factory_type>()(std::declval<char const *>())))
      {
        return first;
      }

      ValueType value() const noexcept(noexcept(std::declval<value_factory_type>()(std::declval<char const *>())))
      {
        return second;
      }

      operator ValueType() const noexcept(noexcept(std::declval<entry>().value()))
      {
        return value();
      }

      operator NameType() const noexcept(noexcept(std::declval<entry>().name()))
      {
        return name();
      }

      constexpr bool operator==(entry const & other) const noexcept
      {
        return m_raw == other.m_raw;
      }

      constexpr operator bool() const noexcept
      {
        return m_raw;
      }

      constexpr udev_list_entry * raw() const noexcept
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

      iterator(udev_list_entry * entry, name_factory_type name_factory, value_factory_type value_factory)
          : m_entry{std::in_place, entry, name_factory, value_factory}
          , m_name_factory{name_factory}
          , m_value_factory{value_factory}
      {
        if (!*m_entry)
        {
          m_entry.reset();
        }
      }

      iterator() = default;

      iterator & operator++() noexcept
      {
        auto next = udev_list_entry_get_next(m_entry->raw());
        if (next)
        {
          m_entry.emplace(next, m_name_factory, m_value_factory);
        }
        else
        {
          m_entry.reset();
        }
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
        return *m_entry;
      }

      private:
      std::optional<entry> m_entry;
      name_factory_type m_name_factory;
      value_factory_type m_value_factory;
    };

    list(
        udev_list_entry * head,
        name_factory_type name_factory = [](auto n) { return NameType{n}; },
        value_factory_type value_factory = [](auto v) { return ValueType{v}; })
        : m_head{head}
        , m_name_factory{std::move(name_factory)}
        , m_value_factory{std::move(value_factory)}
    {
    }

    constexpr iterator begin() const noexcept
    {
      return iterator{m_head, m_name_factory, m_value_factory};
    }

    constexpr iterator end() const noexcept
    {
      return iterator{};
    }

    private:
    udev_list_entry * m_head;
    name_factory_type m_name_factory;
    value_factory_type m_value_factory;
  };

}  // namespace udevxx::detail

#endif