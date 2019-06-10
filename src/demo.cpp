#include <udevxx/udevxx.hpp>

#include <iostream>

int main()
{
  using namespace udevxx::literals;

  auto context = udevxx::context{};
  auto devices = context.devices();

  devices.match("leds"_sub).match(udevxx::initialized);

  for (auto & device : devices)
  {
    std::cout << device.system_path() << "\n\t" << device.subsystem() << "\n\t" << device.system_name() << '\n';
  }
}