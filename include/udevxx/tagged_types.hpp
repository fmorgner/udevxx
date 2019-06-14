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

#ifndef UDEVXX_TAGGED_TYPES_HPP
#define UDEVXX_TAGGED_TYPES_HPP

#include <libudev.h>

#include <filesystem>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

namespace udevxx
{
  struct tagged_type_tag
  {
  };

  struct black_hole
  {
    template <typename... Consumed>
    black_hole(Consumed &&...)
    {
    }
  };

  template <typename UnderlyingType, typename Tag>
  struct tagged_type : tagged_type_tag
  {
    using underlying_type = UnderlyingType;

    explicit tagged_type(UnderlyingType value) noexcept(std::is_nothrow_move_constructible_v<UnderlyingType>)
        : m_real{std::move(value)}
    {
    }

    tagged_type() noexcept(std::is_nothrow_default_constructible_v<UnderlyingType>) = default;

    bool operator<(tagged_type const & other) const
    {
      return m_real < other.m_real;
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

  using action = tagged_type<std::string, struct action_tag>;
  using device_id = tagged_type<std::string, struct device_id_tag>;
  using device_link = tagged_type<std::filesystem::path, struct device_link_tag>;
  using device_node = tagged_type<std::filesystem::path, struct device_node_tag>;
  using device_path = tagged_type<std::string, struct device_path_tag>;
  using device_type = tagged_type<std::string, struct device_type_tag>;
  using driver = tagged_type<std::string, struct driver_tag>;
  using property = tagged_type<std::string, struct property_tag>;
  using subsystem = tagged_type<std::string, struct subsystem_tag>;
  using system_attribute = tagged_type<std::string, struct system_attribute_tag>;
  using system_name = tagged_type<std::string, struct system_name_tag>;
  using system_number = tagged_type<std::string, struct system_number_tag>;
  using system_path = tagged_type<std::filesystem::path, struct system_path_tag>;
  using tag = tagged_type<std::string, struct tag_tag>;

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
      return system_path{std::string{string, length}};
    }

    system_name operator""_name(char const * string, unsigned long length)
    {
      return system_name{{string, length}};
    }

    tag operator""_tag(char const * string, unsigned long length)
    {
      return tag{{string, length}};
    }

    property operator""_prop(char const * string, unsigned long length)
    {
      return property{{string, length}};
    }

    system_attribute operator""_attr(char const * string, unsigned long length)
    {
      return system_attribute{{string, length}};
    }

    device_id operator""_id(char const * string, unsigned long length)
    {
      return device_id{{string, length}};
    }

  }  // namespace literals

}  // namespace udevxx

#endif