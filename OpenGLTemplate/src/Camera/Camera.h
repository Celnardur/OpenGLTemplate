#pragma once

#include "../CoreGraphics/GLIncludes.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Direction { RIGHT, UP, FORWARD };
enum Rotation { PITCH, YAW, ROLL };

#define PI 3.141592653589793

class Camera
{
public:
	Camera() :
		m_vecPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
		m_vecFront(glm::vec3(0.0f, 1.0f, 0.0f)),
		m_vecUp(glm::vec3(0.0f, 0.0f, 1.0f)),
		m_vecRight(glm::vec3(1.0f, 0.0f, 0.0f)),
		m_fFieldOfView(45.0f),
		m_fFarPlane(200.0f)
	{}

	Camera(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 up) :
		m_vecPosition(pos),
		m_fFieldOfView(45.0f),
		m_fFarPlane(200.0f)
	{
		m_vecFront = glm::normalize(lookAt - pos);
		m_vecRight = glm::normalize(glm::cross(m_vecFront, up));
		m_vecUp = glm::normalize(glm::cross(m_vecRight, m_vecFront));
	}

	virtual ~Camera() = default;

	void place(float fXPos, float fYPos, float fZPos = 1.0f);
	void push(float fXShift, float fYShift, float fZShift = 0.0f);
	void push(Direction iDirection, float fDistance);

	virtual void rotate(Rotation iRotation, float fRadians);
	virtual void rotateDegrees(Rotation iRotation, float fDegrees);

	virtual void lookAt(float fXPos, float fYPos, float fZPos);

	virtual void zoomIn(float fMag);
	virtual void zoomOut(float fMag);
	virtual void zoom(float fMag);

	virtual void setFarPlane(float fFarPlane) { m_fFarPlane = fFarPlane; }
	// field of view is in degrees
	virtual void setFieldOfView(float fFieldOfZoom) { m_fFieldOfView = fFieldOfZoom; }

	glm::vec3 getPosition() const { return m_vecPosition; }

	glm::mat4 getViewMatrix() const;
	virtual glm::mat4 getProjectionMatrix(int fWidth, int fHeight) const;

protected:
	glm::vec3 m_vecPosition;

	glm::vec3 m_vecFront;
	glm::vec3 m_vecUp;
	glm::vec3 m_vecRight;

	// field of view is in degrees
	float m_fFieldOfView;
	float m_fFarPlane;
};

inline void Camera::place(float fXPos, float fYPos, float fZPos)
{
	m_vecPosition.x = fXPos;
	m_vecPosition.y = fYPos;
	m_vecPosition.z = fZPos;
}

inline void Camera::push(float fXShift, float fYShift, float fZShift)
{
	m_vecPosition.x += fXShift;
	m_vecPosition.y += fYShift;
	m_vecPosition.z += fZShift;
}

inline void Camera::push(Direction iDirection, float fDistance)
{
	switch (iDirection)
	{
	case RIGHT:
		m_vecPosition += m_vecRight * fDistance;
		break;
	case UP:
		m_vecPosition += m_vecUp * fDistance;
		break;
	case FORWARD:
		m_vecPosition += m_vecFront * fDistance;
		break;
	}
}

inline void Camera::rotate(Rotation iRotation, float fRadians)
{
	glm::mat4 rotationMatrix;
	switch (iRotation)
	{
	case PITCH:
		rotationMatrix = glm::rotate(glm::mat4(1.0f), fRadians, m_vecRight);
		m_vecFront = rotationMatrix * glm::vec4(m_vecFront, 1.0f);
		m_vecUp = glm::normalize(glm::cross(m_vecRight, m_vecFront));
		break;
	case YAW:
		rotationMatrix = glm::rotate(glm::mat4(1.0f), fRadians, m_vecUp);
		m_vecFront = rotationMatrix * glm::vec4(m_vecFront, 1.0f);
		m_vecRight = glm::normalize(glm::cross(m_vecFront, m_vecUp));
		break;
	case ROLL:
		rotationMatrix = glm::rotate(glm::mat4(1.0f), fRadians, m_vecFront * -1.0f);
		m_vecRight = rotationMatrix * glm::vec4(m_vecRight, 1.0f);
		m_vecUp = glm::normalize(glm::cross(m_vecRight, m_vecFront));
		break;
	}
}

inline void Camera::rotateDegrees(Rotation iRotation, float fDegrees)
{
	this->rotate(iRotation, glm::radians(fDegrees));
}

inline void Camera::lookAt(float fXPos, float fYPos, float fZPos)
{
	m_vecFront = glm::normalize(glm::vec3(fXPos, fYPos, fZPos) - m_vecPosition);
	m_vecRight = glm::normalize(glm::cross(m_vecFront, m_vecUp));
	m_vecUp = glm::normalize(glm::cross(m_vecRight, m_vecFront));
}

inline void Camera::zoomIn(float fMag)
{
	m_fFieldOfView -= fMag;
	if (m_fFieldOfView < 1)
		m_fFieldOfView = 1;
}

inline void Camera::zoomOut(float fMag)
{
	m_fFieldOfView += fMag;
	if (m_fFieldOfView > 60)
		m_fFieldOfView = 60;
}

inline void Camera::zoom(float fMag)
{
	m_fFieldOfView += fMag;
	if (m_fFieldOfView > 60)
		m_fFieldOfView = 60;
	else if (m_fFieldOfView < 1)
		m_fFieldOfView = 1;
}



inline glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_vecPosition, m_vecPosition + m_vecFront, m_vecUp);
}
inline glm::mat4 Camera::getProjectionMatrix(int fWidth, int fHeight) const
{
	return glm::perspective(glm::radians(m_fFieldOfView), float(fWidth) / fHeight, 0.1f, m_fFarPlane);
}