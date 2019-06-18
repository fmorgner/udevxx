#include "devtree/device_tree_model.hpp"

#include <utility>

namespace devtree
{

  device_tree_model_columns::device_tree_model_columns()
  {
    add(m_device);
    add(m_subsystem);
    add(m_path);
  }

  device_tree_model_columns::text_column_type device_tree_model_columns::device() const noexcept
  {
    return m_device;
  }

  device_tree_model_columns::text_column_type device_tree_model_columns::subsystem() const noexcept
  {
    return m_subsystem;
  }

  device_tree_model_columns::text_column_type device_tree_model_columns::path() const noexcept
  {
    return m_path;
  }

  device_tree_model::device_tree_model(Glib::RefPtr<Gtk::TreeStore> store)
      : m_columns{}
      , m_store{std::move(store)}
  {
  }

  Gtk::TreeStore::iterator device_tree_model::add(entry const & entry)
  {
    auto row = m_store->append();
    (*row)[m_columns.device()] = entry.device;
    (*row)[m_columns.subsystem()] = entry.subsystem;
    (*row)[m_columns.path()] = entry.path;
    return row;
  }

  Gtk::TreeStore::iterator device_tree_model::add(Gtk::TreeStore::iterator parent, entry const & entry)
  {
    auto row = m_store->append(parent->children());
    (*row)[m_columns.device()] = entry.device;
    (*row)[m_columns.subsystem()] = entry.subsystem;
    (*row)[m_columns.path()] = entry.path;
    return row;
  }

}  // namespace devtree