#include "Camera.h"

#include "Vec3.h"
#include "Vec4.h"
#include "Mat4.h"

float Radians(float number) {
	return number * static_cast<float>(0.01745329251994329576923690768489);
}

Camera::Camera()
{
	positionCamera = Vec3(0.0f, 0.0f, 3.0f);
	frontCamera = Vec3(0.0f, 0.0f, -1.0f);
	upCamera = Vec3(0.0f, 1.0f, 0.0f);
	pitch = 0.0f;
	yaw = -90.0f;
}

Mat4 Camera::GetViewMatrix()
{
	Mat4 result;
	return result.LookAt(positionCamera, positionCamera + frontCamera, upCamera);
}

Mat4 Camera::GetProjectionMatrix()
{
	Mat4 result;
	return result.Perspective(45.0f, (float)800 / (float)600, 0.1f, 100.0f);
}

void Camera::ReadInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		positionCamera += frontCamera * speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		Vec3 crossResult(frontCamera.CrossProduct(upCamera));
		positionCamera -= crossResult.Normalize() * speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		Vec3 crossResult(frontCamera.CrossProduct(upCamera));
		positionCamera += crossResult.Normalize() * speed;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		positionCamera -= frontCamera * speed;
	}
}

void Camera::MouseInput(sf::Window& window)
{
	float xOffset = sf::Mouse::getPosition(window).x - xPos;
	float yOffset = yPos - sf::Mouse::getPosition(window).y;
	xPos = sf::Mouse::getPosition(window).x;
	yPos = sf::Mouse::getPosition(window).y;

	yaw += (xOffset * 0.3f);
	pitch += (yOffset * 0.3f);

	Vec3 front;
	front.x = cos(Radians(yaw)) * cos(Radians(pitch));
	front.y = sin(Radians(pitch));
	front.z = sin(Radians(yaw)) * cos(Radians(pitch));
	frontCamera = front.Normalize();
}

Vec3 Camera::GetPosition()
{
	return positionCamera;
}

Vec3 Camera::GetFront()
{
	return frontCamera;
}

Mat4 Camera::GetViewSkyBox()
{
	Mat4 result = GetViewMatrix();
	result.data[3][0] = 0;
	result.data[3][1] = 0;
	result.data[3][2] = 0;
	result.data[3][3] = 0;

	return result;
}