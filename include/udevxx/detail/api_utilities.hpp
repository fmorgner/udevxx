#ifndef UDEVXX_DETAIL_API_UTILITIES_HPP
#define UDEVXX_DETAIL_API_UTILITIES_HPP

#include <functional>
#include <utility>

namespace udevxx::detail
{

  template <typename TargetType, typename APIFunction, typename... APIArguments>
  TargetType from_nullable(APIFunction && api_function, APIArguments &&... arguments)
  {
    auto nullable = std::invoke(std::forward<APIFunction>(api_function), std::forward<APIArguments>(arguments)...);
    return nullable ? TargetType{nullable} : TargetType{};
  }

  template <typename APIFunction, typename... APIArguments>
  char const * nullable_c_string(APIFunction && api_function, APIArguments &&... arguments)
  {
    auto nullable = std::invoke(std::forward<APIFunction>(api_function), std::forward<APIArguments>(arguments)...);
    return nullable ? nullable : "";
  }

}  // namespace udevxx::detail

#endif