//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/platform/i_application.h"

#include <stdexcept>
#include <vector>

// Enable the WSI extensions
#if defined(__ANDROID__)
#define VK_USE_PLATFORM_ANDROID_KHR
#elif defined(__linux__)
#define VK_USE_PLATFORM_XLIB_KHR
#elif defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <SDL2/SDL_syswm.h>
#include <SDL2/SDL_vulkan.h>

namespace lsim::platform {
IApplication::IApplication(int argc, char *argv[])
    : window_(CreatWindow()), instance_extensions_(RetrieveExtensions()) {
  (void)argc;
  (void)argv;

  // Use validation layers if this is a debug build
  std::vector<const char *> layers;
#if defined(_DEBUG)
  layers.push_back("VK_LAYER_KHRONOS_validation");
#endif
}

IApplication::~IApplication() {}

void IApplication::Run() {}

SDL_Window *IApplication::CreatWindow() const {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw std::runtime_error("Platform: could not initialize SDL.");
  }
  SDL_Window *window =
      SDL_CreateWindow(name_.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_VULKAN);
  if (window_ == NULL) {
    throw std::runtime_error("Platform: could not create SDL window.");
  }

  return window;
}

std::vector<const char *> IApplication::RetrieveExtensions() const {
  // Get WSI extensions from SDL
  unsigned extension_count;
  if (!SDL_Vulkan_GetInstanceExtensions(window_, &extension_count, NULL)) {
    throw std::runtime_error("Platform: could not get the number of required "
                             "instance extensions from SDL.");
  }
  std::vector<const char *> extensions(extension_count);
  if (!SDL_Vulkan_GetInstanceExtensions(window_, &extension_count,
                                        extensions.data())) {
    throw std::runtime_error("Platform: could not get the names of required "
                             "instance extensions from SDL.");
  }
  return extensions;
}

} // namespace lsim::platform