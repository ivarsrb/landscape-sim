#pragma once
#include <GLFW/glfw3.h>
#include <util/types.h>

// Windowing and event handling class
namespace platform {
class Platform {
public:
    Platform(const t::Size &win_size);
    ~Platform();
    Platform(Platform const&) = delete;
    Platform operator=(Platform const&) = delete;
    bool IsRunning() const;
    void PollEvents() const;
    GLFWwindow* GetWindowHandle() const;
private:
    void Init();
    void Shutdown();
    const t::Size window_size_;
    GLFWwindow* window_ = nullptr;
};
}; // platform