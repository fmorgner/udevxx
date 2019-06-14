/*
 * This file is part of udevxx
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019, IFS Institute for Software, all rights reserved
 * Copyright (c) 2019, HSR Hochschule für Technik Rapperswil, all rights reserved
 * Copyright (c) 2019, Felix Morgner <felix.morgner@hsr.ch>, all rights reserved
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  - Neither the name of the copyright holders nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef UDEVXX_DETAIL_LIST_HPP
#define UDEVXX_DETAIL_LIST_HPP

#include <udevxx/detail/api_utilities.hpp>

#include <libudev.h>

#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <optional>
#include <string>
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

      operator NameType() const noexcept(noexcept(std::declval<entry>().name()))
      {
        return name();
      }

      operator ValueType() const noexcept(noexcept(std::declval<entry>().value()))
      {
        return value();
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