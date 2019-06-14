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

#ifndef UDEVXX_MATCHES_HPP
#define UDEVXX_MATCHES_HPP

#include <udevxx/device.hpp>
#include <udevxx/match_manipulators.hpp>
#include <udevxx/tagged_types.hpp>

#include <libudev.h>

#include <type_traits>

namespace udevxx::detail
{

  template <typename KeyType, typename ManipulatorTag>
  auto constexpr match_map = false;

  template <>
  auto constexpr match_map<subsystem, include_tag> = &udev_enumerate_add_match_subsystem;

  template <>
  auto constexpr match_map<subsystem, exclude_tag> = &udev_enumerate_add_nomatch_subsystem;

  template <>
  auto constexpr match_map<system_name, include_tag> = &udev_enumerate_add_match_sysname;

  template <>
  auto constexpr match_map<tag, include_tag> = &udev_enumerate_add_match_tag;

  template <>
  auto constexpr match_map<device, parent_tag> = &udev_enumerate_add_match_parent;

  template <>
  auto constexpr match_map<property, equals_tag> = &udev_enumerate_add_match_property;

  template <>
  auto constexpr match_map<system_attribute, equals_tag> = &udev_enumerate_add_match_sysattr;

  template <>
  auto constexpr match_map<system_attribute, differs_tag> = &udev_enumerate_add_nomatch_sysattr;

  template <typename KeyType, typename ManipulatorTag>
  auto constexpr is_matchable =
      std::negation_v<std::is_same<bool, std::remove_cv_t<decltype(match_map<KeyType, ManipulatorTag>)>>>;

}  // namespace udevxx::detail

#endif