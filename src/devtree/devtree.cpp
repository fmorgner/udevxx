#include "devtree/builder_utilities.hpp"
#include "devtree/device_tree_model.hpp"
#include "devtree/main_window.hpp"

#include <udevxx/udevxx.hpp>

#include <gtkmm.h>

#include <memory>
#include <utility>

int main(int argc, char ** argv)
{
  using namespace devtree;

  auto app = Gtk::Application::create(argc, argv, "ch.hsr.ifs.devtree");
  auto builder = Gtk::Builder::create_from_resource("/main_window.glade");

  auto model = std::make_shared<device_tree_model>(get_object<Gtk::TreeStore>(builder, "device_model"));

  auto context = udevxx::context{};
  for (udevxx::device const & device : context.devices())
  {
    model->add({
        device.system_name()->c_str(),
        device.subsystem()->c_str(),
        device.system_path()->c_str(),
    });
  }

  auto window = get_widget_derived<main_window>(builder, "main_window");

  app->run(*window);
}