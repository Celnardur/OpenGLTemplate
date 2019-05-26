#pragma once
#include "Camera.h"
#include "../Input.h"


// First Person Camera prevents roll by using a World Up vector.
class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera()
	{
		m_vecPosition = glm::vec3(0.0f, 0.0f, 0.0f);
		m_vecFront = glm::vec3(0.0f, 1.0f, 0.0f);
		m_vecUp = glm::vec3(0.0f, 0.0f, 1.0f);
		m_vecRight = glm::vec3(1.0f, 0.0f, 0.0f);
		m_vecWorldUp = m_vecUp;
		fPitch = asin(glm::length(glm::dot(m_vecFront, m_vecWorldUp)));
		m_fFieldOfView = 45.0f;
		m_fFarPlane = 100.0f;
		m_mat4Projection = glm::perspective(glm::radians(m_fFieldOfView),
			float(Input::getPixelWidth()) / Input::getPixelHeight(), 0.1f, m_fFarPlane);
		m_iMessageQueue = Input::createMessageQueue();
	}

	FirstPersonCamera(glm::vec3 pos, glm::vec3 lookAt, glm::vec3 worldUp)
	: m_vecWorldUp(worldUp)
	{
		m_vecPosition = pos;
		m_vecFront = glm::normalize(lookAt - pos);
		calculateRightUp();
		fPitch = asin(glm::length(glm::dot(m_vecFront, m_vecWorldUp)));
		m_fFieldOfView = 45.0f;
		m_fFarPlane = 100.0f;
		m_mat4Projection = glm::perspective(glm::radians(m_fFieldOfView),
			float(Input::getPixelWidth()) / Input::getPixelHeight(), 0.1f, m_fFarPlane);
		m_iMessageQueue = Input::createMessageQueue();
	}

	void processInput();

	void rotate(Rotation iRotation, float fRadians) override;
	void lookAt(float fXPos, float fYPos, float fZPos) override;

	void zoomIn(float fMag) override;
	void zoomOut(float fMag) override;
	void zoom(float fMag) override;

	void setFarPlane(float fFarPlane) override;
	// field of view is in degrees
	void setFieldOfView(float fFieldOfZoom) override;

	glm::mat4 getProjectionMatrix() const { return m_mat4Projection; }

private:
	glm::vec3 m_vecWorldUp;
	// track fPitch to make sure scree doesn't flip when it passes 90 deg
	float fPitch;
	const float fMaxPitch = glm::radians(89.0f);

	glm::mat4 m_mat4Projection;

	int m_iMessageQueue;


	void calculateRightUp();
};

inline void FirstPersonCamera::calculateRightUp()
{
	m_vecRight = glm::normalize(glm::cross(m_vecFront, m_vecWorldUp));
	m_vecUp = glm::normalize(glm::cross(m_vecRight, m_vecFront));
}

inline void FirstPersonCamera::processInput()
{
	Message msg = {};
	while (Input::getMessage(m_iMessageQueue, &msg, true))
	{
		if (msg.message == FRAMEBUFFERRESIZE)
		{
			m_mat4Projection = glm::perspective(glm::radians(m_fFieldOfView),
				float(msg.iXData) / msg.iYData, 0.1f, m_fFarPlane);
		}
	}
}

inline void FirstPersonCamera::rotate(Rotation iRotation, float fRadians)
{
	glm::mat4 rotationMatrix;
	switch (iRotation)
	{
	case PITCH:
		// checks to limit pitch
		if (fRadians + fPitch > fMaxPitch)
			fRadians = fMaxPitch - fPitch;
		else if (fRadians + fPitch < -fMaxPitch)
			fRadians = -fMaxPitch - fPitch;

		fPitch += fRadians;

		rotationMatrix = glm::rotate(glm::mat4(1.0f), fRadians, m_vecRight);
		m_vecFront = rotationMatrix * glm::vec4(m_vecFront, 1.0f);
		calculateRightUp();
		break;
	case YAW:
		rotationMatrix = glm::rotate(glm::mat4(1.0f), fRadians, m_vecUp);
		m_vecFront = rotationMatrix * glm::vec4(m_vecFront, 1.0f);
		calculateRightUp();
		break;
	}
}

inline void FirstPersonCamera::lookAt(float fXPos, float fYPos, float fZPos)
{
	m_vecFront = glm::normalize(glm::vec3(fXPos, fYPos, fZPos) - m_vecPosition);
	calculateRightUp();
}

inline void FirstPersonCamera::zoomIn(float fMag)
{
	m_fFieldOfView -= fMag;
	if (m_fFieldOfView < 1)
		m_fFieldOfView = 1;
	m_mat4Projection = glm::perspective(glm::radians(m_fFieldOfView),
		float(Input::getPixelWidth()) / Input::getPixelHeight(), 0.1f, m_fFarPlane);
}

inline void FirstPersonCamera::zoomOut(float fMag)
{
	m_fFieldOfView += fMag;
	if (m_fFieldOfView > 60)
		m_fFieldOfView = 60;
	m_mat4Projection = glm::perspective(glm::radians(m_fFieldOfView),
		float(Input::getPixelWidth()) / Input::getPixelHeight(), 0.1f, m_fFarPlane);
}

inline void FirstPersonCamera::zoom(float fMag)
{
	m_fFieldOfView += fMag;
	if (m_fFieldOfView > 60)
		m_fFieldOfView = 60;
	else if (m_fFieldOfView < 1)
		m_fFieldOfView = 1;
	m_mat4Projection = glm::perspective(glm::radians(m_fFieldOfView),
		float(Input::getPixelWidth()) / Input::getPixelHeight(), 0.1f, m_fFarPlane);
}

inline void FirstPersonCamera::setFieldOfView(float fFieldOfZoom)
{
	m_fFieldOfView = fFieldOfZoom;
	if (m_fFieldOfView > 60)
		m_fFieldOfView = 60;
	else if (m_fFieldOfView < 1)
		m_fFieldOfView = 1;
	m_mat4Projection = glm::perspective(glm::radians(m_fFieldOfView),
		float(Input::getPixelWidth()) / Input::getPixelHeight(), 0.1f, m_fFarPlane);
}

inline void FirstPersonCamera::setFarPlane(float fFarPlane)
{
	m_fFarPlane = fFarPlane;
	m_mat4Projection = glm::perspective(glm::radians(m_fFieldOfView),
		float(Input::getPixelWidth()) / Input::getPixelHeight(), 0.1f, m_fFarPlane);
}
