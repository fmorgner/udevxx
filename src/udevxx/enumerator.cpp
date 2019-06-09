#include <udevxx/enumerator.hpp>

#include <utility>

namespace udevxx
{

  enumerator::enumerator(detail::ref_ptr<udev> context)
      : m_context{std::move(context)}
      , m_impl{m_context.get()}
  {
  }

}  // namespace udevxx
