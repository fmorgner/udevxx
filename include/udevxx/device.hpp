/*
 * This file is part of udevxx
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019, IFS Institute for Software, all rights reserved
 * Copyright (c) 2019, HSR Hochschule für Technik Rapperswil, all rights reserved
 * Copyright (c) 2019, Felix Morgner <felix.morgner@hsr.ch>, all rights reserved
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  - Neither the name of the copyright holders nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef UDEVXX_DEVICE_HPP
#define UDEVXX_DEVICE_HPP

#include <udevxx/detail/api_utilities.hpp>
#include <udevxx/detail/list.hpp>
#include <udevxx/detail/raw_type_owner.hpp>
#include <udevxx/detail/thread_aware.hpp>
#include <udevxx/device_kind.hpp>
#include <udevxx/device_number.hpp>
#include <udevxx/tagged_types.hpp>

#include <libudev.h>

#include <filesystem>
#include <map>
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

    struct attribute_adaptor : detail::thread_aware
    {
      attribute_adaptor(device & device, system_attribute const & attribute) noexcept
          : m_device{device}
          , m_attribute{attribute}
      {
      }

      operator system_attribute() const noexcept
      {
        return m_attribute;
      }

      bool operator=(std::string const & value) const noexcept
      {
        check_thread();
        return udev_device_set_sysattr_value(m_device.m_raw, m_attribute->c_str(), value.c_str());
      }

      private:
      device & m_device;
      system_attribute const & m_attribute;
    };

    device(context_type const & context, device_id const & id)
        : device{udev_device_new_from_device_id(context.get(), id->c_str())}
    {
    }

    device(context_type const & context, device_kind kind, device_number number)
        : device{udev_device_new_from_devnum(context.get(), static_cast<char>(kind), static_cast<dev_t>(number))}
    {
    }

    device(context_type const & context, system_path const & path)
        : device{udev_device_new_from_syspath(context.get(), path->c_str())}
    {
    }

    device(context_type const & context, subsystem const & subsystem, system_name const & name)
        : device{udev_device_new_from_subsystem_sysname(context.get(), subsystem->c_str(), name->c_str())}
    {
    }

    udevxx::action action() const
    {
      check_thread();
      return detail::from_nullable<udevxx::action>(udev_device_get_action, m_raw);
    }

    udevxx::device_node device_node() const
    {
      check_thread();
      return detail::from_nullable<udevxx::device_node>(udev_device_get_devnode, m_raw);
    }

    udevxx::device_number device_number() const
    {
      check_thread();
      return detail::from_nullable<udevxx::device_number>(udev_device_get_devnum, m_raw);
    }

    udevxx::device_path device_path() const
    {
      check_thread();
      return detail::from_nullable<udevxx::device_path>(udev_device_get_devpath, m_raw);
    }

    udevxx::device_type device_type() const
    {
      check_thread();
      return detail::from_nullable<udevxx::device_type>(udev_device_get_devtype, m_raw);
    }

    udevxx::driver driver() const
    {
      check_thread();
      return detail::from_nullable<udevxx::driver>(udev_device_get_driver, m_raw);
    }

    bool has(tag const & tag) const
    {
      check_thread();
      return static_cast<bool>(udev_device_has_tag(m_raw, tag->c_str()));
    }

    bool is_initialized() const noexcept
    {
      check_thread();
      return udev_device_get_is_initialized(m_raw) > 0;
    }

    std::vector<device_link> links() const
    {
      check_thread();
      auto property_list = detail::list<device_link, std::string>{udev_device_get_devlinks_list_entry(m_raw)};
      return {property_list.begin(), property_list.end()};
    }

    std::string operator[](property const & property) const
    {
      check_thread();
      return detail::from_nullable<std::string>(udev_device_get_property_value, m_raw, property->c_str());
    }

    std::string operator[](system_attribute const & attribute) const
    {
      check_thread();
      return detail::from_nullable<std::string>(udev_device_get_sysattr_value, m_raw, attribute->c_str());
    }

    attribute_adaptor operator[](system_attribute const & attribute)
    {
      return {*this, attribute};
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

    std::optional<device> parent(udevxx::subsystem const & subsystem, udevxx::device_type const & device_type) const
    {
      check_thread();
      if (auto parent = udev_device_get_parent_with_subsystem_devtype(m_raw, subsystem->c_str(), device_type->c_str()))
      {
        return device{udev_device_ref(parent)};
      }
      return std::nullopt;
    }

    std::map<property, std::string> properties() const
    {
      check_thread();
      auto property_list = detail::list<property, std::string>{udev_device_get_properties_list_entry(m_raw)};
      return {property_list.begin(), property_list.end()};
    }

    udevxx::subsystem subsystem() const
    {
      check_thread();
      return detail::from_nullable<udevxx::subsystem>(udev_device_get_subsystem, m_raw);
    }

    std::vector<system_attribute> system_attributes() const
    {
      check_thread();
      auto attribute_list = detail::list<system_attribute, std::string>{udev_device_get_sysattr_list_entry(m_raw)};
      return {attribute_list.begin(), attribute_list.end()};
    }

    udevxx::system_name system_name() const
    {
      check_thread();
      return detail::from_nullable<udevxx::system_name>(udev_device_get_sysname, m_raw);
    }

    udevxx::system_number system_number() const
    {
      check_thread();
      return detail::from_nullable<udevxx::system_number>(udev_device_get_sysnum, m_raw);
    }

    udevxx::system_path system_path() const
    {
      check_thread();
      return detail::from_nullable<udevxx::system_path>(udev_device_get_syspath, m_raw);
    }

    std::vector<tag> tags() const
    {
      check_thread();
      auto tag_list = detail::list<tag, std::string>{udev_device_get_tags_list_entry(m_raw)};
      return {tag_list.begin(), tag_list.end()};
    }

    private:
    friend struct device_enumerator;

    device(udev_device * raw)
        : detail::raw_type_owner<udev_device>(raw, udev_device_ref, udev_device_unref)
    {
    }
  };  // namespace udevxx
}  // namespace udevxx

#endif