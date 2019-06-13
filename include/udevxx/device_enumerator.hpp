#ifndef UDEVXX_DEVICE_ENUMERATOR_HPP
#define UDEVXX_DEVICE_ENUMERATOR_HPP

#include <udevxx/detail/raw_type_owner.hpp>
#include <udevxx/device.hpp>
#include <udevxx/tagged_types.hpp>

#include <libudev.h>

#include <cstddef>
#include <iostream>
#include <optional>

namespace udevxx
{

  struct device_enumerator : detail::raw_type_owner<udev_enumerate>
  {
    struct iterator
    {
      using iterator_category = std::forward_iterator_tag;
      using value_type = device const;
      using difference_type = std::ptrdiff_t;
      using pointer = value_type *;
      using reference = value_type &;

      iterator(device_enumerator const & enumerator, udev_list_entry * entry)
          : m_enumerator{&enumerator}
          , m_entry{entry}
          , m_device{make_device()}
      {
      }

      iterator & operator++()
      {
        m_entry = udev_list_entry_get_next(m_entry);
        m_device = make_device();
        return *this;
      }

      iterator operator++(int)
      {
        auto current = *this;
        ++*this;
        return current;
      }

      reference operator*() const
      {
        return m_device.value();
      }

      bool operator==(iterator const & other) const noexcept
      {
        return m_enumerator == other.m_enumerator && m_entry == other.m_entry;
      }

      bool operator!=(iterator const & other) const noexcept
      {
        return !(*this == other);
      }

      private:
      std::optional<device> make_device()
      {
        if (m_entry)
        {
          auto path = system_path{udev_list_entry_get_name(m_entry)};
          return device{m_enumerator->m_context, path};
        }
        return std::nullopt;
      }

      device_enumerator const * m_enumerator;
      udev_list_entry * m_entry;
      std::optional<device> m_device;
    };

    device_enumerator(detail::raw_type_owner<udev> const & context)
        : detail::raw_type_owner<udev_enumerate>(
              udev_enumerate_new(context.get()), udev_enumerate_ref, udev_enumerate_unref)
        , m_context{context}
    {
    }

    device_enumerator & match(subsystem const & subsystem)
    {
      udev_enumerate_add_match_subsystem(m_raw, subsystem->c_str());
      return *this;
    }

    iterator begin() const
    {
      udev_enumerate_scan_devices(m_raw);
      return iterator{*this, udev_enumerate_get_list_entry(m_raw)};
    }

    iterator end() const
    {
      return iterator{*this, nullptr};
    }

    private:
    detail::raw_type_owner<udev> m_context;
  };

}  // namespace udevxx

#endif