//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/renderer.h"

#include <vector>

#include <SDL.h>

#include "lsim/base/log.h"
#include "lsim/platform/types.h"
#include "lsim/renderer/vlk/swapchain.h"

namespace lsim::renderer {
Renderer::Renderer(SDL_Window *window, const platform::Settings &settings)
    : instance_(window, settings), debug_messenger_(instance_),
      surface_(instance_.Handle(), window),
      device_(instance_.Handle(), surface_.Handle()),
      swapchain_(device_.Handle(), device_.GPU().Handle(), surface_.Handle(),
                 device_.QueueFamilies(), window) {
  base::Log::Info("renderer", "initialized");
}

vlk::Device &Renderer::Device() { return device_; }

vlk::Swapchain &Renderer::Swapchin() { return swapchain_; }
} // namespace lsim::renderer