#ifndef UDEVXX_DETAIL_REF_PTR_HPP
#define UDEVXX_DETAIL_REF_PTR_HPP

#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <udevxx/detail/memory_api.hpp>
#include <utility>

#include <libudev.h>

namespace udevxx::detail
{

  template <typename UdevOpaqueType>
  struct ref_ptr
  {

    template <typename... ParameterTypes>
    ref_ptr(ParameterTypes... parameters)
        : m_raw{memory_api_v<UdevOpaqueType>.alloc(std::forward<ParameterTypes>(parameters)...)}
    {
    }

    ref_ptr(ref_ptr const & other) noexcept
        : m_raw{memory_api_v<UdevOpaqueType>.retain(other.m_raw)}
    {
    }

    ref_ptr(ref_ptr && other) noexcept
    {
      other.swap(*this);
    }

    ~ref_ptr() noexcept
    {
      memory_api_v<UdevOpaqueType>.release(m_raw);
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