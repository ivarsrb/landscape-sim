#pragma once
#include <GLFW/glfw3.h>
#include <base/types.h>
#include "vlk/types.h"
#include "context.h"
#include "window.h"
#include "pipeline_manager.h"
#include "frame_resource.h"
#include "frame_manager.h"

// General rendering class used to set-up means to render and render data in
// it's pure form (without knowing details of what is being rendered)
namespace renderer {
class Renderer {
public:
    Renderer(vlk::Settings settings, GLFWwindow* window);
    ~Renderer();
    const Context& GetContext() const;
    const Window& GetWindow() const;
    PipelineManager& GetPipelineManager();
    // Used to ensure nothing is currently in use on GPU
    void WaitForGPUIdle() const;
    // Window is resized, new size is passed through parameter
    void Resize();
    // Record command buffer that is currently processed by frame manager
    // Recording should happen between following calls
    void BeginRecordCurrentCommandBuffer();
    void EndRecordCurrentCommandBuffer();
    // Get command buffer from currently CPU processed resources
    const vlk::CommandBuffer& GetCurrentCommandBuffer();
    // Frame is rendered between these calls
    void FrameBegin();
    void FrameEnd();
private:
    // Constant throughout the life of the renderer
    const Context context_;
    // Changed upon window resize (dependant on window siize)
    Window window_;
    PipelineManager pipeline_manager_;
    FrameManager frame_manager_;
};
}; // renderer