#ifndef UDEVXX_DETAIL_MEMORY_API_HPP
#define UDEVXX_DETAIL_MEMORY_API_HPP

#include <udevxx/detail/deferred.hpp>

#include <libudev.h>

#include <stdexcept>
#include <tuple>
#include <utility>

namespace udevxx::detail
{

  template <typename Alloc, typename Retain, typename Release>
  struct memory_api
  {
    constexpr memory_api(Alloc alloc, Retain retain, Release release) noexcept
        : m_functions(alloc, retain, release)
    {
    }

    template <typename... ParameterTypes>
    auto alloc(ParameterTypes... parameters) const
    {
      auto raw = std::get<0>(m_functions)(std::forward<ParameterTypes>(parameters)...);
      return raw ? raw : throw std::runtime_error{"Failed to create implementation"};
    }

    template <typename RawType>
    auto retain(RawType * raw) const noexcept
    {
      return std::get<1>(m_functions)(raw);
    }

    template <typename RawType>
    void release(RawType * raw) const noexcept
    {
      !raw || std::get<2>(m_functions)(raw);
    }

    private:
    std::tuple<Alloc, Retain, Release> const m_functions;
  };

  template <>
  struct memory_api<std::nullptr_t, std::nullptr_t, std::nullptr_t>
  {
    constexpr memory_api(std::nullptr_t, std::nullptr_t, std::nullptr_t)
    {
    }

    template <typename... ParameterTypes>
    auto alloc(ParameterTypes... parameters) const noexcept
    {
      static_assert(deferred_v<ParameterTypes...>, "Missing udev C memory API!");
    }

    template <typename RawType>
    auto retain(RawType *) const noexcept
    {
      static_assert(deferred_v<RawType>, "Missing udev C memory API!");
    }

    template <typename RawType>
    void release(RawType *) const noexcept
    {
      static_assert(deferred_v<RawType>, "Missing udev C memory API!");
    }
  };

  template <typename>
  auto constexpr memory_api_v = memory_api{
      nullptr,  // alloc
      nullptr,  // retain
      nullptr,  // release
  };

  template <>
  auto constexpr memory_api_v<udev> = memory_api{
      udev_new,   // alloc
      udev_ref,   // retain
      udev_unref  // release
  };

  template <>
  auto constexpr memory_api_v<udev_enumerate> = memory_api{
      udev_enumerate_new,   // alloc
      udev_enumerate_ref,   // retain
      udev_enumerate_unref  // release
  };

  template <>
  auto constexpr memory_api_v<udev_device> = memory_api{
      udev_device_new_from_syspath,  // alloc
      udev_device_ref,               // retain
      udev_device_unref,             // release
  };

}  // namespace udevxx::detail

#endif