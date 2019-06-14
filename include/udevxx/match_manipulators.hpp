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

  // clang-format off
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

    constexpr auto is_initialized()
    {
      return initialized;
    }

  }  // namespace manipulators
}  // namespace udevxx

#endif