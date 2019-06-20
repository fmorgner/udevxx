#include "devtree/builder_utilities.hpp"
#include "devtree/device_tree_model.hpp"
#include "devtree/main_controller.hpp"
#include "devtree/main_window.hpp"

#include <gtkmm.h>

#include <algorithm>
#include <memory>
#include <utility>

int main(int argc, char ** argv)
{
  using namespace devtree;

  auto app = Gtk::Application::create(argc, argv, "ch.hsr.ifs.devtree");
  auto builder = Gtk::Builder::create_from_resource("/main_window.glade");

  auto model = device_tree_model::create();
  auto window = get_widget_derived<main_window>(builder, "main_window", model);
  auto controller = main_controller{window, model};

  app->run(*window);
}