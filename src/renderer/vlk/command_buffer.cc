//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/command_buffer.h"

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "lsim/renderer/vlk/command_pool.h"
#include "vulkan_shared.h"
namespace lsim::renderer::vlk {

CommandBuffer::CommandBuffer(const CommandPool &command_pool)
    : command_pool_(command_pool),
      command_buffer_(command_pool_.AllocateCommandBuffer(
          CommandPool::BufferLevel::kPrimary)) {

  base::Log::Info("renderer", "command buffer", "initialized");
}

const VkCommandBuffer &CommandBuffer::Handle() const { return command_buffer_; }

void CommandBuffer::Begin(CommandBuffer::Usage usage) const {
    VkCommandBufferBeginInfo begin_info = {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags = static_cast<VkCommandBufferUsageFlagBits>(usage);
    begin_info.pInheritanceInfo = nullptr; 
    ErrorCheck(vkBeginCommandBuffer(command_buffer_, &begin_info));
}

void CommandBuffer::End() const {
    ErrorCheck(vkEndCommandBuffer(command_buffer_));
}

} // namespace lsim::renderer::vlk