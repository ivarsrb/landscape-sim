#pragma once
#include "vulkan_shared.h"
#include "command_pool.h"
#include <base/types.h>

// Command buffer is not created explicitly, but allocated from command pool
// It is also implicitly destroyed when command pool that created it is destroyed
// therefore calling Free() function is not mandatory
namespace renderer::vlk {
class CommandBuffer {
public:
    // When command buffer recording begins, it needs to be set
    // how buffer is going to be used for driver to optimize
    enum class Usage {
        // Specifies that each recording of the command buffer will only be submitted 
        // once, and the command buffer will be resetand recorded again between each submission.
        kOneTimeSubmit = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        // Specifies that a command buffer can be resubmitted to a queue while it is in the pending state
        kSimultaniousUse = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
    };
    CommandBuffer(const CommandPool& command_pool);
    const VkCommandBuffer& Get() const;
    // Non-mandatory freeing of buffer
    void Free(const VkDevice& device) const;
    // Command beffer recording
    void Begin(Usage usage) const;
    void End() const;
    void BeginRenderPass(const VkRenderPass& render_pass, const VkFramebuffer& frame_buffer,
        const VkExtent2D& render_area_extent) const;
    void EndRenderPass() const;
    void BindGraphicsPipeline(const VkPipeline& pipeline) const;
    void BindVertexBuffer(const VkBuffer& buffer, t::U32 binding_index = 0) const;
    void BindIndexBuffer32(const VkBuffer& buffer) const;
    void Draw(t::U32 vertex_count, t::U32 instance_count, t::U32 first_vertex, t::U32 first_instance) const;
    void DrawIndexed(t::U32 index_count, t::U32 instance_count, t::U32 first_index,
        t::U32 vertex_offset, t::U32 first_instance) const;
    void CopyBuffer(const VkBuffer& src_buffer, const VkBuffer& dst_buffer,
        VkDeviceSize size, VkDeviceSize src_offset = 0, VkDeviceSize dst_offset = 0) const;
    // Bind given descriptor sets to graphics binding point based on given pipeline layout
    // first_set_at_index - is the set number of the first descriptor set to be bound.
    void BindGraphicsDescriptorSet(const VkDescriptorSet& descriptor_set, const VkPipelineLayout& layout, 
        t::U32 first_set_at_index = 0) const;
    // Record pipeline barier.for image memory barrier
    void PipelineImageMemoryBarrier(VkPipelineStageFlags src_stagemask, VkPipelineStageFlags dst_stagemask, VkDependencyFlags dependancy_flags,
        t::U32 image_memory_barrier_count, const VkImageMemoryBarrier* image_memory_barrier) const;
private:
    // Reference to resource this renderable is created with
    const CommandPool& command_pool_;
    VkCommandBuffer command_buffer_ = VK_NULL_HANDLE;;
};
}; // renderer vlk