#include <udevxx/context.hpp>

namespace udevxx
{

  void context::swap(context & other) noexcept
  {
    using std::swap;
    swap(*this, other);
  }

  void swap(context & lhs, context & rhs) noexcept
  {
    lhs.swap(rhs);
  }

  device_enumerator context::devices() const
  {
    return device_enumerator{m_impl};
  }

};  // namespace udevxx
