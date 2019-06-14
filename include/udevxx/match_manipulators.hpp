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

#ifndef UDEVXX_MATCH_MANIPULATORS_HPP
#define UDEVXX_MATCH_MANIPULATORS_HPP

#include <udevxx/tagged_types.hpp>

namespace udevxx
{

  template <typename WrappedType, typename ManipulatorTag>
  struct match_manipulator
  {
    auto * operator-> () noexcept
    {
      return wrapped.operator->();
    }

    auto const * operator-> () const noexcept
    {
      return wrapped.operator->();
    }

    WrappedType wrapped;
  };

  template <typename ManipulatorTag>
  struct match_manipulator<std::string, ManipulatorTag>
  {
    match_manipulator(match_manipulator<char const *, ManipulatorTag> other)
        : wrapped{other.wrapped}
    {
    }

    std::string wrapped;
  };

  // clang-format off
  struct differs_tag{};
  struct equals_tag{};
  struct exclude_tag{};
  struct include_tag{};
  struct parent_tag{};
  // clang-format on

  inline namespace manipulators
  {

    // clang-format off
    struct {} initialized;
    // clang-format on

    template <typename WrappedType>
    constexpr auto in(WrappedType wrapped) noexcept(std::is_nothrow_move_constructible_v<WrappedType>)
    {
      return match_manipulator<WrappedType, include_tag>{std::move(wrapped)};
    }

    template <typename WrappedType>
    constexpr auto not_in(WrappedType wrapped) noexcept(std::is_nothrow_move_constructible_v<WrappedType>)
    {
      return match_manipulator<WrappedType, exclude_tag>{std::move(wrapped)};
    }

    template <typename WrappedType>
    constexpr auto parent(WrappedType wrapped) noexcept(std::is_nothrow_move_constructible_v<WrappedType>)
    {
      return match_manipulator<WrappedType, parent_tag>{std::move(wrapped)};
    }

    template <typename WrappedType>
    constexpr auto value_of(WrappedType wrapped) noexcept(std::is_nothrow_move_constructible_v<WrappedType>)
    {
      return wrapped;
    }

    template <typename WrappedType>
    constexpr auto is(WrappedType wrapped) noexcept(std::is_nothrow_move_constructible_v<WrappedType>)
    {
      return match_manipulator<WrappedType, equals_tag>{std::move(wrapped)};
    }

    template <typename WrappedType>
    constexpr auto is_not(WrappedType wrapped) noexcept(std::is_nothrow_move_constructible_v<WrappedType>)
    {
      return match_manipulator<WrappedType, differs_tag>{std::move(wrapped)};
    }

    constexpr auto is_initialized()
    {
      return initialized;
    }

  }  // namespace manipulators
}  // namespace udevxx

#endif