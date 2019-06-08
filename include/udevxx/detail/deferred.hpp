#ifndef UDEVXX_DETEAIL_DEFERRED_HPP
#define UDEVXX_DETEAIL_DEFERRED_HPP

#include <type_traits>

namespace udevxx::detail
{

  template <typename...>
  struct deferred : std::false_type
  {
  };

  template <typename... Swallowed>
  auto constexpr deferred_v = deferred<Swallowed...>::value;

}  // namespace udevxx::detail

#endif