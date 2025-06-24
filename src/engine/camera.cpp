#include "camera.hpp"
#include <algorithm>

Camera::Camera(void) :
    m_position(0.0f, 0.0f, 0.0f),
    m_rotation(0.0f, 0.0f, 0.0f),
    m_forward(m_defaultForward),
    m_right(m_defaultRight),
    m_up(m_defaultUp),
    m_fov(90.f),
    m_aspectRatio(16.0f / 9.0f),
    m_near(0.1f),
    m_far(1000.0f) {
    Update();
}

Camera::Camera(Camera& cam) :
    m_position(cam.m_position),
    m_rotation(cam.m_rotation),
    m_forward(cam.m_forward),
    m_right(cam.m_right),
    m_up(cam.m_up),
    m_fov(cam.m_fov),
    m_aspectRatio(cam.m_aspectRatio),
    m_near(cam.m_near),
    m_far(cam.m_far) {
    Update();
}

void Camera::SetProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane) {
    m_fov = fov;
    m_aspectRatio = aspectRatio;
    m_near = nearPlane;
    m_far = farPlane;
}

void Camera::MoveAbsolute(const glm::vec3& v) {
    m_position = v;
}

void Camera::MoveRelative(const glm::vec3& v) {
    m_position = m_position + v;
}

void Camera::RotateAbsolute(const glm::vec3& v) {
    m_rotation = v;
}

void Camera::RotateRelative(const glm::vec3& v) {
    m_rotation = m_rotation + v;
}

void Camera::Update(void) {
    m_rotation.x = glm::clamp(m_rotation.x, -89.9f, 89.9f);

    float pitchRad = glm::radians(m_rotation.x);
    float yawRad = glm::radians(m_rotation.y);
    
    m_forward.x = cos(yawRad);
    m_forward.y = sin(yawRad);
    m_forward.z = tan(pitchRad);
    m_forward = glm::normalize(m_forward);

    m_right = glm::normalize(glm::cross(m_forward, m_defaultUp));
    m_up = glm::normalize(glm::cross(m_right, m_forward));

    m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_defaultUp);
    m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_near, m_far);
}
