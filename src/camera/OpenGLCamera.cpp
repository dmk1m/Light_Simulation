#include <camera/OpenGLCamera.h>

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

OpenGLCamera::OpenGLCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_position(position),
    m_worldUp(up),
    m_yaw(yaw),
    m_pitch(pitch)
{
    updateCameraVectors();
}

glm::mat4 OpenGLCamera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void OpenGLCamera::processKeyboard(Camera::Movement direction, float deltaTime)
{
    const float velocity = m_movementSpeed * deltaTime;

    switch (direction)
    {
    case FORWARD:  m_position += m_front * velocity; break;
    case BACKWARD: m_position -= m_front * velocity; break;
    case LEFT:     m_position -= m_right * velocity; break;
    case RIGHT:    m_position += m_right * velocity; break;
    }
}

void OpenGLCamera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    m_yaw += xoffset * m_mouseSensitivity;
    m_pitch += yoffset * m_mouseSensitivity;

    if (constrainPitch)
    {
        m_pitch = std::clamp(m_pitch, -Defaults::MaxPitch, Defaults::MaxPitch);
    }

    updateCameraVectors();
}

void OpenGLCamera::processMouseScroll(float yoffset)
{
    m_zoom = std::clamp(m_zoom - yoffset, Defaults::MinZoom, Defaults::MaxZoom);
}

void OpenGLCamera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    front.y = std::sin(glm::radians(m_pitch));
    front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

float OpenGLCamera::getZoom() const
{
    return m_zoom;
}

glm::vec3 OpenGLCamera::getPosition() const
{
    return m_position;
}

glm::vec3 OpenGLCamera::getFront() const
{
    return m_front;
}
