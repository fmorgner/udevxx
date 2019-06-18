#ifndef DEVTREE_BUILDER_UTILITIES_HPP
#define DEVTREE_BUILDER_UTILITIES_HPP

#include <gtkmm.h>

namespace devtree
{

  template <typename WidgetType, typename... ArgumentTypes>
  auto get_widget_derived(Glib::RefPtr<Gtk::Builder> builder, Glib::ustring const & name, ArgumentTypes &&... arguments)
  {
    WidgetType * widget = nullptr;
    builder->get_widget_derived(name, widget, std::forward<ArgumentTypes>(arguments)...);
    return std::shared_ptr<WidgetType>{widget};
  }

  template <typename ObjectType>
  auto get_object(Glib::RefPtr<Gtk::Builder> builder, Glib::ustring const & name)
  {
    auto object = builder->get_object(name);
    return Glib::RefPtr<ObjectType>::cast_dynamic(object);
  }

}  // namespace devtree

#endif