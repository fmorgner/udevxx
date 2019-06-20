#include "devtree/main_window.hpp"

#include "devtree/builder_utilities.hpp"
#include "devtree/device_tree_model.hpp"

namespace devtree
{
  main_window::main_window(BaseObjectType * window,
                           Glib::RefPtr<Gtk::Builder> builder,
                           Glib::RefPtr<device_tree_model> tree_model)
      : Gtk::ApplicationWindow{window}
      , m_builder{builder}
      , m_tree_model{tree_model}
  {
    initialize_device_tree();
    show_all_children();
  }

  Gtk::TreeView & main_window::device_tree() noexcept
  {
    return *get_raw_widget<Gtk::TreeView>(m_builder, "device_tree_view");
  }

  void main_window::initialize_device_tree() noexcept
  {
    auto & device_tree = this->device_tree();

    device_tree.set_model(m_tree_model);
    device_tree.append_column("Device", m_tree_model->columns.device);
    device_tree.append_column("Subsystem", m_tree_model->columns.subsystem);
    device_tree.append_column("Path", m_tree_model->columns.path);

    for (auto column_idx = 0; column_idx < device_tree.get_n_columns(); ++column_idx)
    {
      auto column = device_tree.get_column(column_idx);
      column->set_resizable(true);
      column->set_sort_column(column_idx);
      column->set_reorderable(true);
    }
  }

}  // namespace devtree