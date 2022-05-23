#include "Camera.hpp"

#include "./Transform.hpp"
#include <algorithm>

void Camera::updateTransform() { transform = glm::lookAt(position, position + front, up); }

void Camera::setSensitivity(float _sensitivity) { sensitivity = _sensitivity; }
float Camera::getSensitivity() { return sensitivity; }

void Camera::setStep(float _step) { step = _step; }
float Camera::getStep() { return step; }

void Camera::setYaw(float _yaw) { yaw = _yaw; }
float Camera::getYaw() { return yaw; }

void Camera::setPitch(float _pitch) { pitch = _pitch; }
float Camera::getPitch() { return pitch; }

void Camera::resetTransform() {
    position = anchor;
    front = dFront; pitch = dPitch; yaw = dYaw;
    rotate({ 0, 0 });
}
glm::mat4 Camera::getTransform() { return transform; }

void Camera::rotate(const glm::vec2 &_offset) {
    glm::vec2 offset{sensitivity * _offset};
    yaw += offset.x;
    if (yaw >= 360.f) yaw -= 360.f;
    if (yaw <= -360.f) yaw += 360.f;

    if (unclip) {
        pitch -= offset.y;
        if (pitch >= 360.f) pitch -= 360.f;
        if (pitch <= -360.f) pitch += 360.f;
    } else
        pitch = std::clamp(pitch - offset.y, -90.f, 90.f);

    glm::vec3 _front{0.f};
    _front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    _front.y = glm::sin(glm::radians(pitch));
    _front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    front = glm::normalize(_front);

    updateTransform();
}

void Camera::move(const glm::vec3& _direction) {
    decltype(_direction) direction{_direction * step};
    position += direction.x * front;
    position += glm::normalize(glm::cross(front, up)) * direction.y;
    updateTransform();
}

void Camera::setView(GLuint program) { Utils::setView(program, transform); }

void Camera::setUnclip(bool _unclip) { unclip = _unclip; }
auto Camera::getUnclip() -> decltype(unclip) { return unclip; }
