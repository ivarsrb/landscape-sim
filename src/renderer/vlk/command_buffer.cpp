#include "command_buffer.h"
#include <base/log.h>

namespace renderer::vlk {
CommandBuffer::CommandBuffer(const CommandPool& command_pool) :
    command_pool_(command_pool),
    command_buffer_(command_pool_.AllocateCommandPrimaryBuffer()) {
    base::Log::Info("Renderer: primary command buffer allocated");
}

const VkCommandBuffer& CommandBuffer::Get() const {
    return command_buffer_;
}

// Freeing is not mandatory because command buffers get destroyed together with comand pool
void CommandBuffer::Free(const VkDevice& device) const {
    vkFreeCommandBuffers(device, command_pool_.Get(), 1, &command_buffer_);
}

// Recording
void CommandBuffer::Begin(Usage usage) const {
    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = static_cast<VkCommandBufferUsageFlagBits>(usage);
    ErrorCheck(vkBeginCommandBuffer(command_buffer_, &begin_info));
}

void CommandBuffer::End() const {
    ErrorCheck(vkEndCommandBuffer(command_buffer_));
}

void CommandBuffer::BeginRenderPass(const VkRenderPass& render_pass, const VkFramebuffer& frame_buffer, 
    const VkExtent2D& render_area_extent) const {
    VkRenderPassBeginInfo render_pass_Info{};
    render_pass_Info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_Info.renderPass = render_pass;
    render_pass_Info.framebuffer = frame_buffer;
    render_pass_Info.renderArea.offset = { 0, 0 };
    // Usually swapchain extent
    render_pass_Info.renderArea.extent = render_area_extent;
    // Clear values to use for VK_ATTACHMENT_LOAD_OP_CLEAR, which used as load operation for color attachment.
    VkClearValue clear_color = { 0.1f, 0.1f, 0.1f, 1.0f };
    render_pass_Info.clearValueCount = 1;
    render_pass_Info.pClearValues = &clear_color;
    vkCmdBeginRenderPass(command_buffer_, &render_pass_Info, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBuffer::EndRenderPass() const {
    vkCmdEndRenderPass(command_buffer_);
}

void CommandBuffer::BindGraphicsPipeline(const VkPipeline& pipeline) const {
    vkCmdBindPipeline(command_buffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
}

void CommandBuffer::BindVertexBuffer(const VkBuffer& buffer, uint32_t binding_index) const {
    VkBuffer vertex_buffers[] = { buffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(command_buffer_, binding_index, 1, vertex_buffers, offsets);
}

void CommandBuffer::BindIndexBuffer32(const VkBuffer& buffer) const {
    VkDeviceSize offset = 0;
    vkCmdBindIndexBuffer(command_buffer_, buffer, offset, VK_INDEX_TYPE_UINT32);
}

void CommandBuffer::Draw(uint32_t vertex_count, uint32_t instance_count, uint32_t first_vertex, 
    uint32_t first_instance) const {
    vkCmdDraw(command_buffer_, vertex_count, instance_count, first_vertex, first_instance);
}

void CommandBuffer::DrawIndexed(uint32_t index_count, uint32_t instance_count, uint32_t first_index,
    uint32_t vertex_offset, uint32_t first_instance) const {
    vkCmdDrawIndexed(command_buffer_, index_count, instance_count, first_index, vertex_offset, first_instance);
}

void CommandBuffer::CopyBuffer(const VkBuffer& src_buffer, const VkBuffer& dst_buffer,
    VkDeviceSize size, VkDeviceSize src_offset, VkDeviceSize dst_offset) const {
    VkBufferCopy copy_region {};
    copy_region.srcOffset = src_offset;
    copy_region.dstOffset = dst_offset;
    copy_region.size = size;
    vkCmdCopyBuffer(command_buffer_, src_buffer, dst_buffer, 1, &copy_region);
}
};