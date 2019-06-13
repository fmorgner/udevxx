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