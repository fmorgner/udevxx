#include <udevxx/context.hpp>

#include <iostream>

int main()
{
  using namespace udevxx::literals;

  auto context = udevxx::context{};
  auto devices = context.devices();

  devices.match("leds"_sub);

  for (auto & device : devices)
  {
    std::cout << device.system_path() << "\n\t" << device.subsystem() << '\n';
  }
}