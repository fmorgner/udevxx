#ifndef UDEVXX_DETAIL_LIST_HPP
#define UDEVXX_DETAIL_LIST_HPP

#include <libudev.h>

#include <cstddef>
#include <functional>
#include <iterator>
#include <optional>

namespace udevxx::detail
{

  template <typename NameType, typename ValueType>
  struct list
  {
    using name_factory_type = std::function<NameType(char const *)>;
    using value_factory_type = std::function<ValueType(char const *)>;

    struct entry
    {
      entry(udev_list_entry * raw, name_factory_type name_factory, value_factory_type value_factory) noexcept
          : m_raw{raw}
          , m_name_factory{std::move(name_factory)}
          , m_value_factory{std::move(value_factory)}
      {
      }

      NameType name() const noexcept(noexcept(std::declval<name_factory_type>()(std::declval<char const *>())))
      {
        auto raw_name = udev_list_entry_get_name(m_raw);
        return m_name_factory(raw_name ? raw_name : "");
      }

      ValueType value() const noexcept(noexcept(std::declval<value_factory_type>()(std::declval<char const *>())))
      {
        auto raw_value = udev_list_entry_get_value(m_raw);
        return m_name_factory(raw_value ? raw_value : "");
      }

      operator ValueType() const noexcept(noexcept(std::declval<entry>().value()))
      {
        return value();
      }

      operator NameType() const noexcept(noexcept(std::declval<entry>().name()))
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
      name_factory_type m_name_factory;
      value_factory_type m_value_factory;
    };

    struct iterator
    {
      using iterator_category = std::forward_iterator_tag;
      using difference_type = std::ptrdiff_t;
      using value_type = entry const;
      using reference = value_type &;
      using pointer = value_type *;

      iterator(udev_list_entry * entry, name_factory_type name_factory, value_factory_type value_factory)
          : m_entry{entry, name_factory, value_factory}
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
      return iterator{nullptr, m_name_factory, m_value_factory};
    }

    private:
    udev_list_entry * m_head;
    name_factory_type m_name_factory;
    value_factory_type m_value_factory;
  };

}  // namespace udevxx::detail

#endif