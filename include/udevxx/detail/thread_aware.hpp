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

#ifndef UDEVXX_DETAIL_THREAD_AWARE_HPP
#define UDEVXX_DETAIL_THREAD_AWARE_HPP

#include <stdexcept>
#include <thread>

namespace udevxx::detail
{
  struct thread_aware
  {

    thread_aware() noexcept
        : m_thread_id{std::this_thread::get_id()}
    {
    }

    thread_aware(thread_aware const & other) noexcept
        : m_thread_id{other.m_thread_id}
    {
      check_thread();
    }

    thread_aware(thread_aware && other) noexcept
        : m_thread_id{other.m_thread_id}
    {
      check_thread();
    }

    ~thread_aware()
    {
      check_thread();
    }

    thread_aware & operator=(thread_aware const & other) noexcept
    {
      check_thread();
      return *this;
    }

    thread_aware & operator=(thread_aware && other) noexcept
    {
      check_thread();
      return *this;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wexceptions"
    void check_thread() const noexcept
    {
      if (m_thread_id != std::this_thread::get_id())
      {
        throw std::logic_error{"libudev objects may not be shared across threads, even if all access is synchronized "
                               "(see man 3 libudev)!"};
      }
    }
#pragma GCC diagnostic pop

    private:
    std::thread::id m_thread_id;
  };
}  // namespace udevxx::detail

#endif