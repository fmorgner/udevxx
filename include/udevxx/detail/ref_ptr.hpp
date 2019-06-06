#ifndef UDEVXX_DETAIL_REF_PTR
#define UDEVXX_DETAIL_REF_PTR

#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

#include <libudev.h>

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

  template <typename...>
  auto constexpr defer_v = false;

  template <>
  struct memory_api<std::nullptr_t, std::nullptr_t, std::nullptr_t>
  {
    constexpr memory_api(std::nullptr_t, std::nullptr_t, std::nullptr_t)
    {
    }

    template <typename... ParameterTypes>
    auto alloc(ParameterTypes... parameters)
    {
      static_assert(defer_v<ParameterTypes...>, "Missing udev C memory API!");
    }

    template <typename RawType>
    auto retain(RawType *)
    {
      static_assert(defer_v<RawType>, "Missing udev C memory API!");
    }

    template <typename RawType>
    void release(RawType *)
    {
      static_assert(defer_v<RawType>, "Missing udev C memory API!");
    }
  };

  template <typename>
  auto constexpr udev_c_memory_api = memory_api{
      nullptr,  // alloc
      nullptr,  // retain
      nullptr,  // release
  };

  template <>
  auto constexpr udev_c_memory_api<udev> = memory_api{
      udev_new,   // alloc
      udev_ref,   // retain
      udev_unref  // release
  };

  template <>
  auto constexpr udev_c_memory_api<udev_enumerate> = memory_api{
      udev_enumerate_new,   // alloc
      udev_enumerate_ref,   // retain
      udev_enumerate_unref  // release
  };

  template <typename UdevOpaqueType>
  struct ref_ptr
  {
    template <typename... ParameterTypes>
    ref_ptr(ParameterTypes... parameters)
        : m_raw{udev_c_memory_api<UdevOpaqueType>.alloc(std::forward<ParameterTypes>(parameters)...)}
    {
    }

    ref_ptr(ref_ptr const & other) noexcept
        : m_raw{udev_c_memory_api<UdevOpaqueType>.retain(other.m_raw)}
    {
    }

    ref_ptr(ref_ptr && other) noexcept
    {
      other.swap(*this);
    }

    ~ref_ptr() noexcept
    {
      udev_c_memory_api<UdevOpaqueType>.release(m_raw);
    }

    ref_ptr & operator=(ref_ptr other) noexcept
    {
      swap(other);
      return *this;
    }

    void swap(ref_ptr & other) noexcept
    {
      std::swap(m_raw, other.m_raw);
    }

    private:
    UdevOpaqueType * m_raw{};
  };

  template <typename UdevOpaqueType, typename RefPtr = ref_ptr<UdevOpaqueType>>
  void swap(RefPtr & lhs, RefPtr & rhs)
  {
    lhs.swap(rhs);
  }

}  // namespace udevxx::detail

#endif