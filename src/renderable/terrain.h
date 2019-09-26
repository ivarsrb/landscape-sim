#pragma once
#include <vector>
#include "i_renderable.h"
#include <renderer/types.h>
#include <renderer/renderer.h>
#include <renderer/vlk/vertex_buffer.h>
#include <renderer/vlk/index_buffer.h>
#include <base/matrix.h>

struct UniformBufferObject {
    t::Mat4 world_from_local;
    t::Mat4 view_from_world;
    t::Mat4 projection_from_view;
};

// Terrain representation as a renderable object
namespace renderable {
class Terrain : public IRenderable {
public:
    Terrain(renderer::Renderer& renderer);
    virtual void AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer) const override;
private:
    renderer::vlk::GraphicsPipeline::CreateParams GetPipelineDescription();
    // Generate and return height grid populated with height values that define
    // terrain height.
    base::Matrix<t::F32> GenerateHeightGrid(t::U16 size) const;
    std::vector<renderer::VertexPos2dColor> GetVertices() const;
    std::vector<t::U32> GetIndices() const;
    // Height grid representation as 2d matrix
    const base::Matrix<t::F32> height_grid_;
    // Reference to renderer this triangle is tied with
    renderer::Renderer& renderer_;
    // Id of pipeline that is going to be used for rendering
    // Can't store pipeline handle directly, because pipelines are recreated upon screen
    // resize. But id is tied to specific location in pipeline catche
    const renderer::PipelineId pipeline_id_;
    const std::vector<renderer::VertexPos2dColor> vertices_;
    const std::vector<t::U32> indices_;
    const renderer::vlk::VertexBuffer vertex_buffer_;
    const renderer::vlk::IndexBuffer index_buffer_;
};
}; // renderable