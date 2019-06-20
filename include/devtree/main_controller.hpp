#ifndef DEVTREE_MAIN_CONTROLLER
#define DEVTREE_MAIN_CONTROLLER

#include "devtree/device_tree_model.hpp"
#include "devtree/main_window.hpp"

#include <gtkmm.h>

#include <memory>

namespace devtree
{

  struct main_controller
  {
    using tree_model_type = Glib::RefPtr<device_tree_model>;
    using main_window_type = std::shared_ptr<main_window>;

    main_controller(main_window_type window, tree_model_type tree_model);

    private:
    main_window_type m_window;
    tree_model_type m_tree_model;
  };

}  // namespace devtree

#endif