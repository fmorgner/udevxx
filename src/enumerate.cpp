#include <udevxx/udevxx.hpp>

#include <iostream>

using namespace udevxx::literals;
using namespace udevxx::manipulators;

int main()
{
  auto context = udevxx::context{};
  auto devices = context.devices().match(initialized).match("leds"_sub);

  for (auto const & device : devices)
  {
    std::cout << device.subsystem() << '\n';
  }
}