#pragma once
#include <camera/Camera.h>

#include <glm/glm.hpp>

// Fly-through camera driven by Euler angles (yaw/pitch).
class FlyCamera : public Camera {
public:
	struct Defaults {
		static constexpr float Yaw = -90.0f;
		static constexpr float Pitch = 0.0f;
		static constexpr float Speed = 2.5f;
		static constexpr float Sensitivity = 0.1f;
		static constexpr float Zoom = 45.0f;     // field of view in degrees
		static constexpr float MinZoom = 1.0f;
		static constexpr float MaxZoom = 45.0f;
		static constexpr float MaxPitch = 89.0f;
	};

	explicit FlyCamera(
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = Defaults::Yaw,
		float pitch = Defaults::Pitch
	);

	glm::mat4 getViewMatrix() const override;

	void processKeyboard(Camera::Movement direction, float deltaTime) override;
	void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true) override;
	void processMouseScroll(float yoffset) override;
	float getZoom() const override;
	glm::vec3 getPosition() const override;
	glm::vec3 getFront() const override;

private:
	glm::vec3 m_position;
	glm::vec3 m_front{ 0.0f, 0.0f, -1.0f };
	glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
	glm::vec3 m_right{ 1.0f, 0.0f, 0.0f };
	glm::vec3 m_worldUp;

	float m_yaw;
	float m_pitch;

	float m_movementSpeed = Defaults::Speed;
	float m_mouseSensitivity = Defaults::Sensitivity;
	float m_zoom = Defaults::Zoom;

	// Recalculates front/right/up from the current yaw and pitch.
	void updateCameraVectors();
};
