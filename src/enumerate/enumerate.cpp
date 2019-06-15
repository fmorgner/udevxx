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

#include <udevxx/udevxx.hpp>

#include <iostream>
#include <map>
#include <vector>

using namespace udevxx::literals;
using namespace udevxx::manipulators;

inline namespace support
{
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

}  // namespace support

inline namespace traverse
{
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
    indent(out, depth) << prefix << ": " << device.system_path()->native() << '\n';
    indent(out, depth + 1) << "subsystem: " << device.subsystem() << '\n';
    indent(out, depth + 1) << "sysname: " << device.system_name() << '\n';
    indent(out, depth + 1) << "sysnum: " << device.system_number() << '\n';
    indent(out, depth + 1) << "devnum: " << device.device_number() << '\n';
    indent(out, depth + 1) << "devpath: " << device.device_path() << '\n';
    indent(out, depth + 1) << "devtype: " << device.device_type() << '\n';
    indent(out, depth + 1) << "devnode: " << device.device_node()->native() << '\n';
    indent(out, depth + 1) << "driver: " << device.driver() << '\n';
    indent(out, depth + 1) << "initialized: " << (device.is_initialized() ? "yes" : "no") << '\n';
    indent(out, depth + 1) << "action: " << device.action() << '\n';
    indent(out, depth + 1) << "links: " << device.action() << '\n';
    map(device.links(), [&](auto const & path) { indent(out, depth + 2) << "- " << path->native() << '\n'; });
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

}  // namespace traverse

int main()
{
  auto context = udevxx::context{};

  auto devices = context.devices();

  for (auto const & device : devices)
  {
    visit(device, print);
  }
}