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