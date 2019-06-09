#include <udevxx/device.hpp>

#include <utility>

namespace udevxx
{

  using namespace detail;

  device::device(ref_ptr<udev> context, std::string syspath)
      : m_context{std::move(context)}
      , m_impl{m_context.get(), syspath.c_str()}
  {
  }

  std::string device::subsystem() const
  {
    return udev_device_get_subsystem(m_impl.get());
  }

  std::string device::system_path() const
  {
    return udev_device_get_syspath(m_impl.get());
  }

};  // namespace udevxx