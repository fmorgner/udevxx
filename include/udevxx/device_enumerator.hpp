#ifndef UDEVXX_DEVICE_ENUMERATOR_HPP
#define UDEVXX_DEVICE_ENUMERATOR_HPP

#include <udevxx/detail/matches.hpp>
#include <udevxx/detail/raw_type_owner.hpp>
#include <udevxx/detail/thread_aware.hpp>
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

  struct device_enumerator
      : detail::raw_type_owner<udev_enumerate>
      , detail::thread_aware
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

    bool add(system_path const & path)
    {
      return udev_enumerate_add_syspath(m_raw, path->c_str()) >= 0;
    }

    iterator begin() const
    {
      check_thread();
      udev_enumerate_scan_devices(m_raw);
      using namespace std::placeholders;
      m_devices = device_list{udev_enumerate_get_list_entry(m_raw), [&](auto path) {
                                return device{m_context, system_path{path}};
                              }};
      return m_devices.begin();
    }

    iterator end() const
    {
      check_thread();
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

    template <typename... MatchTypes>
    device_enumerator match(MatchTypes &&... matches)
    {
      auto filtered_enumerator = device_enumerator{m_context};
      return filtered_enumerator.do_match(std::forward<MatchTypes>(matches)...);
    }

    private:
    template <typename MatchKey,
              typename MatchManipulatorTag,
              typename = std::enable_if_t<detail::is_matchable<MatchKey, MatchManipulatorTag>>>
    device_enumerator do_match(match_manipulator<MatchKey, MatchManipulatorTag> const & matcher)
    {
      check_thread();
      if constexpr (std::is_same_v<typename MatchKey::underlying_type, std::string>)
      {
        detail::match_map<MatchKey, MatchManipulatorTag>(m_raw, matcher->c_str());
      }
      return *this;
    }

    template <typename MatchManipulatorTag,
              typename = std::enable_if_t<detail::is_matchable<device, MatchManipulatorTag>>>
    device_enumerator do_match(match_manipulator<device, MatchManipulatorTag> const & matcher)
    {
      check_thread();
      detail::match_map<device, MatchManipulatorTag>(m_raw, matcher.wrapped.m_raw);
      return *this;
    }

    template <typename TaggedType,
              typename = std::enable_if_t<std::is_base_of_v<tagged_type_tag, TaggedType>>,
              typename = std::enable_if_t<detail::is_matchable<TaggedType, include_tag>>>
    device_enumerator do_match(TaggedType matcher)
    {
      return do_match(match_manipulator<TaggedType, include_tag>{matcher});
    }

    template <typename KeyType,
              typename ValueType,
              typename MatchManipulatorTag,
              typename StringValueType = std::conditional_t<std::is_same_v<char const *, std::remove_cv_t<ValueType>>,
                                                            std::string,
                                                            std::remove_cv_t<ValueType>>,
              typename = std::enable_if_t<detail::is_matchable<KeyType, MatchManipulatorTag>>>
    device_enumerator do_match(KeyType const & key, match_manipulator<ValueType, MatchManipulatorTag> value)
    {
      check_thread();
      auto match_function = detail::match_map<KeyType, MatchManipulatorTag>;
      if constexpr (std::is_same_v<std::remove_cv_t<ValueType>, StringValueType>)
      {
        match_function(m_raw, key->c_str(), value.wrapped.c_str());
      }
      else
      {
        match_function(m_raw, key->c_str(), value.wrapped);
      }
      return *this;
    }

    device_enumerator do_match(decltype(initialized))
    {
      check_thread();
      udev_enumerate_add_match_is_initialized(m_raw);
      return *this;
    }

    detail::raw_type_owner<udev> m_context;
    device_list mutable m_devices;
  };

}  // namespace udevxx

#endif