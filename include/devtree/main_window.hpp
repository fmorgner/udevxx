#ifndef DEVTREE_MAIN_WINDOW_HPP
#define DEVTREE_MAIN_WINDOW_HPP

#include <gtkmm.h>

namespace devtree
{

  struct main_window : Gtk::ApplicationWindow
  {
    main_window(BaseObjectType * window, Glib::RefPtr<Gtk::Builder> builder);

    private:
    Glib::RefPtr<Gtk::Builder> m_builder;
    Gtk::TreeView * m_device_tree;
  };

}  // namespace devtree

#endif