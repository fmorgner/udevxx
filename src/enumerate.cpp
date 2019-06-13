#include <udevxx/udevxx.hpp>

#include <iostream>

using namespace udevxx::literals;

int main()
{
  auto context = udevxx::context{};
  auto devices = context.devices();

  for (auto const & device : devices.match("leds"_sub))
  {
    std::cout << device.system_path()->c_str() << '\n';
  }
}