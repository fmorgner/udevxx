#include <gtkmm.h>
#include <iostream>

int main(int argc, char ** argv)
{
  auto app = Gtk::Application::create(argc, argv, "ch.hsr.ifs.devtree");
  auto builder = Gtk::Builder::create_from_resource("/main_window.glade");

  Gtk::Window * main_window;
  builder->get_widget("window_main_window", main_window);
  app->run(*main_window);
  delete main_window;
}