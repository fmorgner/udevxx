#include <udevxx/device.hpp>

#include <utility>

namespace udevxx
{

  using namespace detail;

  device::device(ref_ptr<udev> context, syspath path)
      : m_context{std::move(context)}
      , m_impl{m_context.get(), path->c_str()}
  {
  }

  subsystem device::subsystem() const
  {
    return ::udevxx::subsystem{udev_device_get_subsystem(m_impl.get())};
  }

  syspath device::system_path() const
  {
    return syspath{udev_device_get_syspath(m_impl.get())};
  }

};  // namespace udevxx