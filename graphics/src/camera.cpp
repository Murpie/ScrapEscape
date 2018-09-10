#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera()
{
}

Camera::Camera(float fovy, float width,
	float height, float near, float far)
	: projection(glm::perspective(fovy, width / height, near, far))
{
	yaw = -80.0f;
	pitch = 0.0f;
	position = glm::vec3(0.0f, 0.0f, 1.0f);
	forward = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::~Camera()
{
}

void Camera::update(std::chrono::milliseconds delta, const input & i)
{
	using namespace glm;
	
	float speed{ 10.f };
	vec3 offset{ 0.0f, 0.0f, 0.0f };
	float dt = delta.count() / 1000.0f;
	
	if (i.state(button::up) == button_state::pressed)
	{
		offset += vec3{ 0, speed, 0 } * dt;
	}
	if (i.state(button::left) == button_state::pressed)
	{
		offset += vec3{ -speed, 0, 0 } * dt;
	}
	if (i.state(button::down) == button_state::pressed)
	{
		offset += vec3{ 0, -speed, 0 } * dt;
	}
	if (i.state(button::right) == button_state::pressed)
	{
		offset += vec3{ speed, 0, 0 } * dt;
	}

	position += offset;
}



glm::mat4 Camera::projection_matrix() const
{
	return projection;
}

glm::mat4 Camera::view_matrix() const
{
	return glm::lookAt(position, position + forward, up);
}

void Camera::set_projection(const glm::mat4& projection)
{
	this->projection = projection;
}

void Camera::mouse_movement(const glm::vec2& mouse_pos)
{
	if (!initialized)
	{
		last_mouse_position.x = mouse_pos.x;
		last_mouse_position.y = mouse_pos.y;
		initialized = true;
	}

	auto xoffset = mouse_pos.x - last_mouse_position.x;
	auto yoffset = last_mouse_position.y - mouse_pos.y;
	last_mouse_position.x = mouse_pos.x;
	last_mouse_position.y = mouse_pos.y;

	auto sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	front.y = glm::sin(glm::radians(pitch));
	front.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	forward = glm::normalize(front);
}