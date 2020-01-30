#include "perspective_camera.h"
// The perspective projection matrix generated by GLM will use the OpenGL depth range of -1.0 to 1.0 by default. 
// Force to use the Vulkan range of 0.0 to 1.0.
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <base/log.h>
#include "types.h"

namespace scene {
PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera::Parameters& params) :
    ICamera((params.world_translation)),
    z_near_(params.znear),
    z_far_(params.zfar),
    y_field_of_view_(params.yfov),
    // Points at negative Z axis by default
    yaw_(0),
    pitch_(0),
    // 1.4 m/s human walking speed
    movement_speed_(1.0f),
    mouse_sensitivity_(0.25f) {
    UpdateVectors(yaw_, pitch_);
    base::Log::Info("Scene: perspective camera initialized : position - " , glm::to_string(world_position_), ", znear - ",
        z_near_, ", zfar - ", z_far_, ", yfov - ", y_field_of_view_);
}

void PerspectiveCamera::Move(const platform::Input& input, t::F32 dt) {
    // Camera controls using active keys (keys neing currently held)
    if (input.GetKeyData().active.count(platform::Input::Key::kW))
        UpdatePosition(Direction::kForward, dt);
    if (input.GetKeyData().active.count(platform::Input::Key::kS))
        UpdatePosition(Direction::kBackward, dt);
    if (input.GetKeyData().active.count(platform::Input::Key::kA))
        UpdatePosition(Direction::kLeft, dt);
    if (input.GetKeyData().active.count(platform::Input::Key::kD))
        UpdatePosition(Direction::kRight, dt);
    if (input.GetKeyData().active.count(platform::Input::Key::kR))
        UpdatePosition(Direction::kUp, dt);
    if (input.GetKeyData().active.count(platform::Input::Key::kF))
        UpdatePosition(Direction::kDown, dt);
}

void PerspectiveCamera::Rotate(const platform::Input& input, bool constrain_pitch) {
    bool free_look_enabled = input.GetMouseData().cursor_disabled;
    if (free_look_enabled) {
        auto offset = input.GetMouseData().offset * mouse_sensitivity_;
        // Make sure yaw_ stays between 0 and 360
        yaw_ = glm::mod(yaw_ + offset.x, 360.0f);
        pitch_ += offset.y;
        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrain_pitch) {
            constexpr t::F32 kConstraint = 89.0;
            if (pitch_ > kConstraint) {
                pitch_ = kConstraint;
            }
            else
                if (pitch_ < -kConstraint) {
                    pitch_ = -kConstraint;
                }
        }
        UpdateVectors(yaw_, pitch_);
    }
}

t::Mat4 PerspectiveCamera::GetViewMatrix() const {
    return glm::lookAt(world_position_, world_position_ + front_, up_);
}

t::Mat4 PerspectiveCamera::GetProjectionMatrix(t::F32 aspect_ratio) const {
    t::Mat4 projection;
    projection =  glm::perspective(y_field_of_view_, aspect_ratio, z_near_, z_far_);
    // For image not to appear upsde down compensade for flipped y clip coordinate
    projection[1][1] *= -1;
    return projection;
}

// Calculate front, right and up vectors from current Euler angles
void PerspectiveCamera::UpdateVectors(t::F32 yaw, t::F32 pitch) {
    // Calculate the new Front vector
    t::Vec3 front;
    // TODO: work with radians not degrees
    front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front_ = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    right_ = glm::normalize(glm::cross(front_, kWorldUpVec));
    up_ = glm::normalize(glm::cross(right_, front_));
}

void PerspectiveCamera::UpdatePosition(Direction direction, t::F32 dt) {
    // Since this is animation, mediate it by elapsed time
    t::F32 velocity = movement_speed_ * dt;
    switch (direction) {
    case Direction::kForward:
        world_position_ += front_ * velocity;
        break;
    case Direction::kBackward:
        world_position_ -= front_ * velocity;
        break;
    case Direction::kLeft:
        world_position_ -= right_ * velocity;
        break;
    case Direction::kRight:
        world_position_ += right_ * velocity;
        break;
    case Direction::kUp:
        world_position_ += up_ * velocity;
        break;
    case Direction::kDown:
        world_position_ -= up_ * velocity;
        break;
    }
}

}; // scene