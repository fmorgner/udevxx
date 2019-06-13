#include <udevxx/udevxx.hpp>

#include <iostream>
#include <vector>

using namespace udevxx::literals;
using namespace udevxx::manipulators;

template <typename ScopedType, typename Callable>
void map(std::vector<ScopedType> const & scoped, Callable callable)
{
  if (!scoped.empty())
  {
    for (auto const & value : scoped)
    {
      callable(value);
    }
  }
}

template <typename ScopedType, typename Callable>
void map(std::optional<ScopedType> const & scoped, Callable callable)
{
  if (scoped)
  {
    callable(scoped.value());
  }
}

void visit(udevxx::device const & device,
           void (*callable)(udevxx::device const &, int, std::ostream &),
           std::ostream & out = std::cout,
           int depth = 0)
{
  callable(device, depth, out);
}

std::ostream & indent(std::ostream & out, int depth)
{
  for (int i = 0; i < depth; ++i)
  {
    out << '\t';
  }
  return out;
}

void print(udevxx::device const & device, int depth, std::ostream & out = std::cout)
{
  indent(out, depth) << "device: " << device.system_path() << '\n';
  indent(out, depth + 1) << "subsystem: " << device.subsystem() << '\n';
  indent(out, depth + 1) << "sysname: " << device.system_name() << '\n';
  indent(out, depth + 1) << "tags: \n";
  map(device.tags(), [&](auto const & tag) { indent(out, depth + 2) << "- " << tag << '\n'; });
  map(device.parent(), [&](auto const & parent) { visit(parent, print, out, depth + 1); });
}

int main()
{
  auto context = udevxx::context{};
  auto devices = context.devices().match(in("leds"_sub));

  for (auto const & device : devices)
  {
    visit(device, print);
  }
}