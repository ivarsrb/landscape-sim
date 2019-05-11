#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace renderer::vlk {
// OS dependant surface where images are presented to
// Supported OS:
// * Windows
class Surface {
public:
    Surface(const VkInstance& instance, GLFWwindow* window);
    ~Surface();
    Surface(Surface const&) = delete;
    Surface operator=(Surface const&) = delete;
    const VkSurfaceKHR& Get() const;
private:
    VkSurfaceKHR Create(const VkInstance& instance, GLFWwindow* window) const;
    // We grab reference and store it here because we need it in destructor
    const VkInstance& instance_;
    // Images are presented to surface
    const VkSurfaceKHR surface_ = VK_NULL_HANDLE;
};
}; // renderer vlk