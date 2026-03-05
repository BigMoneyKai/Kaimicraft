#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include "util.h"

// ================= Physics =================
constexpr float g = 0.05f;

// ================= Movement =================
constexpr float walkSpeed   = 0.05f;
constexpr float runSpeed    = 0.09f;
constexpr float jumpSpeed   = 0.05f;

// ================= Fly =================
constexpr float flyUpSpeed   = 0.05f;
constexpr float flyDownSpeed = 0.05f;

// ================= Crouch =================
constexpr float crouchSpeed = 0.05f;
constexpr float getUpSpeed  = 0.05f;

// ================= Default Camera =================
constexpr float defaultFov  = 70.0f;
constexpr float defaultNear = 0.1f;
constexpr float defaultFar  = 1000.0f;

class Camera {
public:
    void init();

    const glm::vec3& pos()   const { return m_pos; }
    float yaw()   const { return m_yaw; }
    float pitch() const { return m_pitch; }
    float roll()  const { return m_roll; }

    float fov()  const { return m_fov; }
    float far()  const { return m_far; }
    float near() const { return m_near; }

    const glm::vec3& front() const { return m_front; }
    const glm::vec3& right() const { return m_right; }
    const glm::vec3& up()    const { return m_up; }

    void setPos(const glm::vec3& pos) { m_pos = pos; }
    void setYaw(float yaw)            { m_yaw = yaw; }
    void setPitch(float pitch)        { m_pitch = pitch; }
    void setRoll(float roll)          { m_roll = roll; }
    void setFov(float fov)            { m_fov = fov; }
    void setFar(float far)            { m_far = far; }
    void setNear(float near)          { m_near = near; }

    void addYaw(float delta);
    void addPitch(float delta);
    void addRoll(float delta);
    void addFov(float delta);
    void move(const glm::vec3& delta);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjMatrix(float aspect) const;

    void updateVectors();
    
    void destroy();
public:
    glm::vec3 m_pos; 
    float m_yaw; // OpenGL facing -Z
    float m_pitch;
    float m_roll;

    float m_fov;
    float m_far;
    float m_near;

    glm::vec3 m_front;
    glm::vec3 m_right;
    glm::vec3 m_up;

    glm::vec3 m_worldUp;
};
