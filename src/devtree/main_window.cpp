#include "devtree/main_window.hpp"

#include "devtree/builder_utilities.hpp"
#include "devtree/device_tree_model.hpp"

#include <iostream>

namespace devtree
{
  main_window::main_window(BaseObjectType * window, Glib::RefPtr<Gtk::Builder> builder)
      : Gtk::ApplicationWindow{window}
      , m_builder{builder}
      , m_detail_view{get_object<Gtk::Grid>(m_builder, "detail_view")}
      , m_device_tree{}
      , m_paned{}
  {
    g_assert(m_detail_view.get());
    m_builder->get_widget("device_tree_view", m_device_tree);
    g_assert(m_device_tree);
    m_builder->get_widget("main_window_panes", m_paned);
    g_assert(m_paned);

    m_device_tree->signal_row_activated().connect([&](auto const & path, auto * column) {
      auto current_detail_view = m_paned->get_child2();
      if (!current_detail_view)
      {
        m_paned->add2(*m_detail_view.get());
      }
      // Gtk::Revealer * sidebar = nullptr;
      // m_builder->get_widget("sidebar", sidebar);
      // g_assert(sidebar);
      // sidebar->set_reveal_child(true);
    });
  }

}  // namespace devtree