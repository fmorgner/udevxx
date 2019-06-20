#include "devtree/device_tree_model.hpp"

#include <utility>

namespace devtree
{

  Glib::RefPtr<device_tree_model> device_tree_model::create()
  {
    return Glib::RefPtr{new device_tree_model()};
  }

  device_tree_model::device_tree_model()
  {
    columns.add(columns.device);
    columns.add(columns.subsystem);
    columns.add(columns.path);

    set_column_types(columns);
  }

  Gtk::TreeStore::iterator device_tree_model::add(entry const & entry)
  {
    auto row = append();
    (*row)[columns.device] = entry.device;
    (*row)[columns.subsystem] = entry.subsystem;
    (*row)[columns.path] = entry.path;
    return row;
  }

  Gtk::TreeStore::iterator device_tree_model::add(Gtk::TreeStore::iterator parent, entry const & entry)
  {
    auto row = append(parent->children());
    (*row)[columns.device] = entry.device;
    (*row)[columns.subsystem] = entry.subsystem;
    (*row)[columns.path] = entry.path;
    return row;
  }

}  // namespace devtree