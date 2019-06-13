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

namespace udevxx
{

  struct device_enumerator : detail::raw_type_owner<udev_enumerate>
  {
    using iterator = std::vector<device>::iterator;

    device_enumerator(detail::raw_type_owner<udev> const & context)
        : detail::raw_type_owner<udev_enumerate>(
              udev_enumerate_new(context.get()), udev_enumerate_ref, udev_enumerate_unref)
        , m_context{context}
        , m_devices{}
    {
    }

    iterator begin() const
    {
      udev_enumerate_scan_devices(m_raw);
      auto devices = detail::list<system_path, void>{udev_enumerate_get_list_entry(m_raw)};
      for (auto const & entry : devices)
      {
        m_devices.emplace_back(m_context, entry);
      }
      m_devices.shrink_to_fit();
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
      m_devices.clear();

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
      m_devices.clear();
      udev_enumerate_add_match_is_initialized(m_raw);
      return *this;
    }

    private:
    detail::raw_type_owner<udev> m_context;
    std::vector<device> mutable m_devices;
  };

}  // namespace udevxx

#endif