#ifndef DEVTREE_MAIN_WINDOW_HPP
#define DEVTREE_MAIN_WINDOW_HPP

#include <gtkmm.h>

#include <memory>

namespace devtree
{

  struct main_window : Gtk::ApplicationWindow
  {
    main_window(BaseObjectType * window, Glib::RefPtr<Gtk::Builder> builder);

    private:
    Glib::RefPtr<Gtk::Builder> m_builder;
    Glib::RefPtr<Gtk::Grid> m_detail_view;

    Gtk::TreeView * m_device_tree;
    Gtk::Paned * m_paned;
  };

}  // namespace devtree

#endif