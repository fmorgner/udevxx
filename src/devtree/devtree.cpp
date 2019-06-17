#include <gtkmm.h>
#include <iostream>
#include <memory>

template <typename WidgetType>
auto get_widget(Glib::RefPtr<Gtk::Builder> builder, Glib::ustring const & name)
{
  WidgetType * widget = nullptr;
  builder->get_widget(name, widget);
  return std::shared_ptr<WidgetType>{widget};
}

int main(int argc, char ** argv)
{
  auto app = Gtk::Application::create(argc, argv, "ch.hsr.ifs.devtree");
  auto builder = Gtk::Builder::create_from_resource("/main_window.glade");
  auto main_window = get_widget<Gtk::ApplicationWindow>(builder, "main_window");

  app->run(*main_window);
}