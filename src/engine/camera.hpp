#pragma once
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    Camera(void);
    Camera(Camera& cam);
    void SetProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);
    void MoveAbsolute(const glm::vec3& v);
    void MoveRelative(const glm::vec3& v);
    void RotateAbsolute(const glm::vec3& v);
    void RotateRelative(const glm::vec3& v);
    void Update(void);
    inline const glm::mat4x4& GetViewMatrix(void) const { return m_viewMatrix; }
    inline const glm::mat4x4& GetProjectionMatrix(void) const { return m_projectionMatrix; }
    
    inline const glm::vec3& GetPositionVector(void) const { return m_position; }
    inline const glm::vec3& GetRotationVector(void) const { return m_rotation; }
    
    inline const glm::vec3& GetForwardVector(void) const { return m_forward; }
    inline const glm::vec3& GetDefaultForwardVector(void) const { return m_defaultForward; }
    inline const glm::vec3& GetRightVector(void) const { return m_right; }
    inline const glm::vec3& GetDefaultRightVector(void) const { return m_defaultRight; }
    inline const glm::vec3& GetUpVector(void) const { return m_up; }
    inline const glm::vec3& GetDefaultUpVector(void) const { return m_defaultUp; }

private:
    glm::mat4x4 m_viewMatrix;
    glm::mat4x4 m_projectionMatrix;
    glm::vec3 m_position;
    glm::vec3 m_rotation; // Pitch, Yaw, Roll
    glm::vec3 m_forward;
    glm::vec3 m_right;
    glm::vec3 m_up;
    float m_fov;
    float m_aspectRatio;
    float m_near;
    float m_far;
    const glm::vec3 m_defaultForward   = {  0.0f, 1.0f, 0.0f };  // +Y
    const glm::vec3 m_defaultRight     = { -1.0f, 0.0f, 0.0f };  // -X
    const glm::vec3 m_defaultUp        = {  0.0f, 0.0f, 1.0f };  // +Z
};