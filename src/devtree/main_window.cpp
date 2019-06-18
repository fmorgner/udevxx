#include "devtree/main_window.hpp"

#include "devtree/device_tree_model.hpp"

#include <iostream>

namespace devtree
{
  main_window::main_window(BaseObjectType * window, Glib::RefPtr<Gtk::Builder> builder)
      : Gtk::ApplicationWindow{window}
      , m_builder{builder}
      , m_device_tree{}
  {
    m_builder->get_widget("device_tree", m_device_tree);
    g_assert(m_device_tree);
  }

}  // namespace devtree