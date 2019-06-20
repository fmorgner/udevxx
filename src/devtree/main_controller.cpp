#include "devtree/main_controller.hpp"

#include <udevxx/udevxx.hpp>

#include <utility>

namespace devtree
{

  main_controller::main_controller(main_controller::main_window_type window,
                                   main_controller::tree_model_type tree_model)
      : m_window{std::move(window)}
      , m_tree_model{std::move(tree_model)}
  {
    g_assert(m_window);
    g_assert(m_tree_model);

    for (udevxx::device && device : udevxx::context{}.devices())
    {
      m_tree_model->add({
          device.system_name()->c_str(),
          device.subsystem()->c_str(),
          device.system_path()->c_str(),
      });
    }
  }

}  // namespace devtree