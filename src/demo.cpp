#include <udevxx/context.hpp>

#include <iostream>

int main()
{
  auto context = udevxx::context{};
  auto devices = context.devices();

  for (auto & device : devices)
  {
    std::cout << device.system_path() << "\n\t" << device.subsystem() << '\n';
  }
}