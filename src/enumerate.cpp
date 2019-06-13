#include <udevxx/udevxx.hpp>

#include <iostream>

using namespace udevxx::literals;
using namespace udevxx::manipulators;

int main()
{
  auto context = udevxx::context{};
  auto devices = context.devices();

  for (auto const & device : devices)
  {
    // clang-format off
    std::cout << device.system_path()
              << "\n\tsubsystem: " << device.subsystem()
              << "\n\tsysname: " << device.system_name()
              << '\n';
    // clang-format on
  }
}