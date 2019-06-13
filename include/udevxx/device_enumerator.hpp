#ifndef UDEVXX_DEVICE_ENUMERATOR_HPP
#define UDEVXX_DEVICE_ENUMERATOR_HPP

#include <udevxx/detail/matches.hpp>
#include <udevxx/detail/raw_type_owner.hpp>
#include <udevxx/device.hpp>
#include <udevxx/match_manipulators.hpp>
#include <udevxx/tagged_types.hpp>

#include <libudev.h>

#include <cstddef>
#include <iostream>
#include <optional>
#include <string>

namespace udevxx
{

  struct device_enumerator : detail::raw_type_owner<udev_enumerate>
  {
    using device_list = detail::list<device, std::string>;
    using iterator = device_list::iterator;

    device_enumerator(detail::raw_type_owner<udev> const & context)
        : detail::raw_type_owner<udev_enumerate>(
              udev_enumerate_new(context.get()), udev_enumerate_ref, udev_enumerate_unref)
        , m_context{context}
        , m_devices{nullptr, nullptr, nullptr}
    {
    }

    iterator begin() const
    {
      udev_enumerate_scan_devices(m_raw);
      using namespace std::placeholders;
      m_devices = device_list{udev_enumerate_get_list_entry(m_raw), [&](auto path) {
                                return device{m_context, system_path{path}};
                              }};
      return m_devices.begin();
    }

    iterator end() const
    {
      return m_devices.end();
    }

    iterator cbegin() const
    {
      return begin();
    }

    iterator cend() const
    {
      return end();
    }

    template <typename MatchKey,
              typename MatchManipulatorTag,
              typename = std::enable_if_t<detail::is_matchable<MatchKey, MatchManipulatorTag>>>
    device_enumerator & match(match_manipulator<MatchKey, MatchManipulatorTag> const & matcher)
    {
      if constexpr (std::is_same_v<typename MatchKey::underlying_type, std::string>)
      {
        detail::match_map<MatchKey, MatchManipulatorTag>(m_raw, matcher->c_str());
      }
      return *this;
    }

    template <typename TaggedType,
              typename = std::enable_if_t<std::is_base_of_v<tagged_type_tag, TaggedType>>,
              typename = std::enable_if_t<detail::is_matchable<TaggedType, include_tag>>>
    device_enumerator & match(TaggedType matcher)
    {
      return match(match_manipulator<TaggedType, include_tag>{matcher});
    }

    device_enumerator & match(decltype(initialized))
    {
      udev_enumerate_add_match_is_initialized(m_raw);
      return *this;
    }

    private:
    device make_device(char const * path)
    {
      return device{m_context, system_path{path}};
    }

    detail::raw_type_owner<udev> m_context;
    device_list mutable m_devices;
  };

}  // namespace udevxx

#endif