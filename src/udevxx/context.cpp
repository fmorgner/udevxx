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

  enumerator context::devices() const
  {
    return enumerator{m_impl};
  }

};  // namespace udevxx
