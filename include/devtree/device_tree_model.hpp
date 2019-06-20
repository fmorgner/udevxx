#ifndef DEVTREE_DEVICE_TREE_MODEL_HPP
#define DEVTREE_DEVICE_TREE_MODEL_HPP

#include <gtkmm.h>

#include <memory>

namespace devtree
{

  struct device_tree_model : Gtk::TreeStore
  {

    struct columns : Gtk::TreeModelColumnRecord
    {
      Gtk::TreeModelColumn<Glib::ustring> device{};
      Gtk::TreeModelColumn<Glib::ustring> subsystem{};
      Gtk::TreeModelColumn<Glib::ustring> path{};
    } columns;

    struct entry
    {
      Glib::ustring const device{};
      Glib::ustring const subsystem{};
      Glib::ustring const path{};
    };

    static Glib::RefPtr<device_tree_model> create();

    Gtk::TreeStore::iterator add(entry const & entry);
    Gtk::TreeStore::iterator add(Gtk::TreeStore::iterator parent, entry const & entry);

    private:
    device_tree_model();
  };

}  // namespace devtree

#endif