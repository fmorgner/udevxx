#include <udevxx/context.hpp>

#include <iostream>

int main()
{
  auto context = udevxx::context{};
  auto devices = context.devices();

  std::cout << "Hello, udev!\n";
}