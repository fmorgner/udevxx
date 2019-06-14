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

#ifndef UDEVXX_DETAIL_RAW_TYPE_OWNER
#define UDEVXX_DETAIL_RAW_TYPE_OWNER

#include <stdexcept>
#include <utility>

namespace udevxx::detail
{

  template <typename RawType>
  struct raw_type_owner
  {
    using retain_type = RawType * (*)(RawType *);
    using release_type = RawType * (*)(RawType *);

    raw_type_owner(RawType * raw, retain_type retain, release_type release)
        : m_raw{raw ? raw : throw std::runtime_error{"Failed to acquire C library handle"}}
        , m_retain{retain}
        , m_release{release}
    {
    }

    raw_type_owner(raw_type_owner const & other) noexcept
        : m_raw{other.m_retain(other.m_raw)}
        , m_retain{other.m_retain}
        , m_release{other.m_release}
    {
    }

    raw_type_owner(raw_type_owner && other) noexcept
        : m_raw{other.m_raw}
        , m_retain{other.m_retain}
        , m_release{other.m_release}
    {
      other.m_raw = nullptr;
    }

    ~raw_type_owner() noexcept
    {
      m_release(m_raw);
    }

    raw_type_owner & operator=(raw_type_owner const & other) noexcept
    {
      auto copy = other;
      swap(*this, copy);
    }

    raw_type_owner & operator=(raw_type_owner && other) noexcept
    {
      swap(*this, other);
      return *this;
    }

    RawType * get() const noexcept
    {
      return m_raw;
    }

    friend void swap(raw_type_owner & lhs, raw_type_owner & rhs) noexcept
    {
      std::swap(lhs.m_raw, rhs.m_raw);
    }

    protected:
    RawType * m_raw;
    retain_type m_retain;
    release_type m_release;
  };

}  // namespace udevxx::detail

#endif