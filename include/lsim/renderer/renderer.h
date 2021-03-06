//
// Created by Ivars Rusbergs in 2021
//
// Main context of the renderer.
// Stores renderer state and functions.
#include <vector>

#include <SDL.h>

#include "lsim/platform/types.h"
#include "vlk/debug_messenger.h"
#include "vlk/device.h"
#include "vlk/instance.h"
#include "vlk/surface.h"
#include "vlk/swapchain.h"

namespace lsim::renderer {
class Renderer {
public:
  Renderer(SDL_Window *window, const platform::Settings &settings);
  // Get vulkan device abstraction object
  [[nodiscard]] vlk::Device& Device();
  // Get vulkan swapchain abstraction object
  [[nodiscard]] vlk::Swapchain& Swapchin();
private:
  vlk::Instance instance_;
  // Responsible for validation info feedback output.
  // Dependent on Instance being already initialized.
  const vlk::DebugMessenger debug_messenger_;
  vlk::Surface surface_;
  vlk::Device device_;
  vlk::Swapchain swapchain_;
};

} // namespace lsim::renderer