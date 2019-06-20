#ifndef DEVTREE_MAIN_WINDOW_HPP
#define DEVTREE_MAIN_WINDOW_HPP

#include <gtkmm.h>

#include <memory>

namespace devtree
{

  struct main_window : Gtk::ApplicationWindow
  {
    main_window(BaseObjectType * window,
                Glib::RefPtr<Gtk::Builder> builder,
                Glib::RefPtr<struct device_tree_model> tree_model);

    Gtk::TreeView & device_tree() noexcept;
    Gtk::ListBox & detail_list_box() noexcept;

    private:
    void initialize_device_tree() noexcept;

    Glib::RefPtr<Gtk::Builder> m_builder;
    Glib::RefPtr<device_tree_model> m_tree_model;
  };

}  // namespace devtree

#endif