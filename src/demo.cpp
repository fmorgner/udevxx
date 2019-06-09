#include <udevxx/context.hpp>

#include <iostream>

int main()
{
  auto context = udevxx::context{};
  auto devices = context.devices();

  for (auto & device : devices)
  {
    std::cout << "device: " << device.system_path() << "\n\tsubsystem:" << device.subsystem() << '\n';
  }

  std::cout << "Hello, udev!\n";
}