#include <iostream>
#include <udevxx/context.hpp>

int main()
{
  auto ctx = udevxx::context{};
  auto ct2 = ctx;
  auto ct3 = std::move(ct2);

  std::cout << "Hello, udev!\n";
}