#include "camera.h"

void Camera::init() {
    m_pos = glm::vec3(0.0f, 0.0f, 0.0f);
    m_yaw = glm::radians(-90.0f);
    m_pitch = glm::radians(0.0f);
    m_roll = glm::radians(0.0f);

    m_fov = glm::radians(defaultFov);
    m_far = defaultFar;
    m_near = defaultNear;

    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_right = glm::vec3(1.0f, 0.0f, 0.0f);
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);

    m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::addYaw(float delta) {
    m_yaw += delta;
    updateVectors();
}

void Camera::addPitch(float delta) {
    m_pitch += delta;
    m_pitch = std::clamp(m_pitch, glm::radians(-89.0f), glm::radians(89.0f));
    updateVectors();
}

void Camera::addRoll(float delta) {
    m_roll += delta;
    m_roll = std::clamp(m_roll, glm::radians(-89.0f), glm::radians(89.0f));
    updateVectors();
}

void Camera::addFov(float delta) {
    m_fov += delta;
    updateVectors();
}   

void Camera::move(const glm::vec3& delta) {
    m_pos += delta;
}

void Camera::updateVectors() {
    glm::vec3 front;
    front.x = cos(m_yaw) * cos(m_pitch);
    front.y = sin(m_pitch);
    front.z = sin(m_yaw) * cos(m_pitch);

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::destroy() {

}
