#ifndef DEVTREE_DEVICE_TREE_MODEL_HPP
#define DEVTREE_DEVICE_TREE_MODEL_HPP

#include <gtkmm.h>

#include <memory>

namespace devtree
{

  struct device_tree_model_columns : Gtk::TreeModelColumnRecord
  {
    using text_column_type = Gtk::TreeModelColumn<Glib::ustring>;

    device_tree_model_columns();

    text_column_type device() const noexcept;
    text_column_type subsystem() const noexcept;
    text_column_type path() const noexcept;

    private:
    text_column_type m_device{};
    text_column_type m_subsystem{};
    text_column_type m_path{};
  };

  struct device_tree_model
  {
    struct entry
    {
      Glib::ustring device;
      Glib::ustring subsystem;
      Glib::ustring path;
    };

    using impl_type = Glib::RefPtr<Gtk::TreeStore>;

    device_tree_model(impl_type store);

    Gtk::TreeStore::iterator add(entry const & entry);
    Gtk::TreeStore::iterator add(Gtk::TreeStore::iterator parent, entry const & entry);

    private:
    device_tree_model_columns m_columns;
    impl_type m_store;
  };

}  // namespace devtree

#endif