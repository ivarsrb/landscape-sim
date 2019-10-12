#include "view.h"
#include <vector>
#include <base/log.h>
#include "types.h"

namespace scene {
View::View(renderer::Renderer& renderer) :
    renderer_(renderer),
    descr_set_layout_(AddDescrSetLayout()),
    uniform_buffer_id_(AddUniformBufferId()),
    pipeline_layout_dummy_(renderer_.GetContext().device.Get(), renderer::vlk::PipelineLayout::Params{ {descr_set_layout_.Get()} }) {
    base::Log::Info("Scene: view initialized");
}

void View::InitDescriptorSet() {
    // Add descriptor sets (inside sets for all frame-in-flights are created)
    descr_set_id_ = renderer_.GetShaderResources().AddDescriptorSet(descr_set_layout_.Get());
    // Bind uniform buffer to descriptor set
    renderer_.GetShaderResources().UpdateDescriptorSetWithUniformBuffer(descr_set_id_, uniform_buffer_id_);
}

void View::UpdateUniformBuffer(renderer::FrameManager::FrameId frame_id, const ICamera& camera) const {
    UniformData ubo{};
    ubo.view_from_world = camera.GetViewMatrix();
    ubo.projection_from_view = camera.GetProjectionMatrix(renderer_.GetWindow().GetAspectRatio());
    renderer_.GetShaderResources().GetkUniformBuffer(uniform_buffer_id_, frame_id).Update(&ubo);
}

// Per-view shader resources are bound once per view, so this resource is not necessery
// to bind per-object basis. 
// This function should be called at the top of a rendering loop.
void View::BindDescriptorSet(const renderer::vlk::CommandBuffer& command_buffer, renderer::FrameManager::FrameId frame_id) const {
    command_buffer.BindGraphicsDescriptorSet(renderer_.GetShaderResources().GetDescriptorSet(descr_set_id_, frame_id).Get(),
        pipeline_layout_dummy_.Get(), DescruptorSetSlotId::kPerView);
}

const renderer::vlk::DescriptorSetLayout& View::AddDescrSetLayout() const {
    std::vector<renderer::vlk::DescriptorSetLayout::Binding> bindings;
    renderer::vlk::DescriptorSetLayout::Binding uniform_binding;
    // Binding index (location), corresponds to layout(binding = n)  in shader
    uniform_binding.index = ViewDescriptorBidingId::kUniformBuffer;
    uniform_binding.type = renderer::vlk::DescriptorType::kUniformBuffer;
    uniform_binding.stage = renderer::vlk::ShaderStage::kVertex;
    uniform_binding.count = 1;
    bindings.push_back(uniform_binding);
    return renderer_.GetShaderResources().GetDescriptorSetLayoutCache().AddDescriptorSetLayout(bindings);
}

t::U64 View::AddUniformBufferId() const {
    return renderer_.GetShaderResources().AddUniformBuffer("uniform buffer view", sizeof(UniformData));
}
}; // scene