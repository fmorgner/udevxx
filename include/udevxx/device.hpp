#ifndef UDEVXX_DEVICE_HPP
#define UDEVXX_DEVICE_HPP

#include <udevxx/detail/list.hpp>
#include <udevxx/detail/raw_type_owner.hpp>
#include <udevxx/detail/thread_aware.hpp>
#include <udevxx/tagged_types.hpp>

#include <libudev.h>

#include <optional>
#include <string>
#include <vector>

namespace udevxx
{
  struct device
      : detail::raw_type_owner<udev_device>
      , detail::thread_aware
  {
    using context_type = detail::raw_type_owner<udev>;

    device(context_type const & context, system_path const & path)
        : device{udev_device_new_from_syspath(context.get(), path->c_str())}
    {
    }

    udevxx::system_path system_path() const
    {
      check_thread();
      auto path = udev_device_get_syspath(m_raw);
      return udevxx::system_path{{path ? path : ""}};
    }

    udevxx::subsystem subsystem() const
    {
      check_thread();
      auto sub = udev_device_get_subsystem(m_raw);
      return udevxx::subsystem{{sub ? sub : ""}};
    }

    udevxx::system_name system_name() const
    {
      check_thread();
      auto name = udev_device_get_sysname(m_raw);
      return udevxx::system_name{{name ? name : ""}};
    }

    udevxx::device_path device_path() const
    {
      check_thread();
      auto path = udev_device_get_devpath(m_raw);
      return udevxx::device_path{{path ? path : ""}};
    }

    udevxx::device_node device_node() const
    {
      check_thread();
      auto node = udev_device_get_devnode(m_raw);
      return udevxx::device_node{{node ? node : ""}};
    }

    std::vector<tag> tags() const
    {
      check_thread();
      auto tag_list = detail::list<tag, std::string>{udev_device_get_tags_list_entry(m_raw)};
      return {tag_list.begin(), tag_list.end()};
    }

    std::optional<device> parent() const
    {
      check_thread();
      if (auto parent = udev_device_get_parent(m_raw))
      {
        return device{udev_device_ref(parent)};
      }
      return std::nullopt;
    }

    private:
    device(udev_device * raw)
        : detail::raw_type_owner<udev_device>(raw, udev_device_ref, udev_device_unref)
    {
    }
  };  // namespace udevxx
}  // namespace udevxx

#endif