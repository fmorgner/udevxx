#ifndef UDEVXX_DEVICE_ENUMERATOR_HPP
#define UDEVXX_DEVICE_ENUMERATOR_HPP

#include <udevxx/detail/device_iterator.hpp>
#include <udevxx/detail/ref_ptr.hpp>
#include <udevxx/device.hpp>

#include <libudev.h>

#include <iterator>
#include <utility>

namespace udevxx
{

  struct initialized_t
  {
    constexpr explicit initialized_t() noexcept = default;
  };

  static auto constexpr initialized = initialized_t{};

  struct device_enumerator
  {
    friend struct context;

    using iterator = detail::device_iterator;

    /**
     * @brief Get an iterator to the first device matched by this enumerator
     *
     * @note This might be the same as enumerator::end if the enumerator does not match any device.
     */
    iterator begin() const noexcept;

    /**
     * @brief Get the past-the-end iterator of this enumerator
     */
    iterator end() const noexcept;

    /**
     * @brief Match only devices in the given subsystem
     *
     * Restrict devices matched by this enumerator to include only devices in the kernel subsystem provided.
     *
     * @param subsystem The subsystem to match against
     */
    device_enumerator & match(subsystem subsystem);

    /**
     * @brief Match only device not in the given subsystem
     *
     * Restrict devices matched by this enumerator to exclude devices in the kernel subsystem provided.
     *
     * @param subsystem The subsystem to match against
     */
    device_enumerator & dont_match(subsystem subsystem);

    /**
     * @brief Match only initialized devices
     *
     * @param initialized A tag for selection this match
     */
    device_enumerator & match(initialized_t initialized);

    private:
    explicit device_enumerator(detail::ref_ptr<udev> context);

    detail::ref_ptr<udev> const m_context;
    detail::ref_ptr<udev_enumerate> const m_impl;
  };

}  // namespace udevxx

#endif