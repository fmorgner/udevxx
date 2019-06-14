#include <udevxx/udevxx.hpp>

#include <iostream>
#include <map>
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

template <typename KeyType, typename ValueType, typename Callable>
void map(std::map<KeyType, ValueType> const & scoped, Callable callable)
{
  for (auto const & value : scoped)
  {
    callable(value);
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
           void (*callable)(udevxx::device const &, char const *, int, std::ostream &),
           char const * prefix = "device",
           std::ostream & out = std::cout,
           int depth = 0)
{
  callable(device, prefix, depth, out);
}

std::ostream & indent(std::ostream & out, int depth)
{
  for (int i = 0; i < depth; ++i)
  {
    out << '\t';
  }
  return out;
}

void print(udevxx::device const & device, char const * prefix, int depth, std::ostream & out = std::cout)
{
  indent(out, depth) << prefix << ": " << device.system_path() << '\n';
  indent(out, depth + 1) << "subsystem: " << device.subsystem() << '\n';
  indent(out, depth + 1) << "sysname: " << device.system_name() << '\n';
  indent(out, depth + 1) << "sysnum: " << device.system_number() << '\n';
  indent(out, depth + 1) << "devnum: " << device.device_number() << '\n';
  indent(out, depth + 1) << "devpath: " << device.device_path() << '\n';
  indent(out, depth + 1) << "devtype: " << device.device_type() << '\n';
  indent(out, depth + 1) << "driver: " << device.driver() << '\n';
  indent(out, depth + 1) << "initialized: " << (device.is_initialized() ? "yes" : "no") << '\n';
  indent(out, depth + 1) << "action: " << device.action() << '\n';
  indent(out, depth + 1) << "tags: \n";
  map(device.tags(), [&](auto const & tag) { indent(out, depth + 2) << "- " << tag << '\n'; });
  indent(out, depth + 1) << "properties: \n";
  map(device.properties(),
      [&](auto const & entry) { indent(out, depth + 2) << entry.first << ": " << entry.second << '\n'; });
  indent(out, depth + 1) << "attributes: \n";
  map(device.system_attributes(),
      [&](auto const & attr) { indent(out, depth + 2) << attr << " => " << device[attr] << '\n'; });
  map(device.parent(), [&](auto const & parent) { visit(parent, print, "parent", out, depth + 1); });
}

int main()
{
  auto context = udevxx::context{};
  auto devices = context.devices();

  for (auto const & device : devices)
  {
    visit(device, print);
  }
}