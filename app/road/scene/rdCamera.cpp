#include "rdCamera.h"
#include <qmath.h>

rdCamera::~rdCamera()
{
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
QMatrix4x4 rdCamera::getViewMatrix()
{
	QMatrix4x4 vm;
	vm.lookAt(Position, Position + Front, Up);

	return vm;
}

QMatrix4x4 rdCamera::getProjectionMatrix()
{
	return m_mProjection;
}

void rdCamera::setProjectionMatrix(QMatrix4x4 matrix)
{
	m_mProjection = matrix;
}

float rdCamera::getOffsetZ()
{
	return m_offsetZ;
}

void rdCamera::setOffsetZ(float z)
{
	m_offsetZ = z;
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void rdCamera::processKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;


	// make sure the user stays at the ground level
//	Position.y = 0.0f; // <-- this one-liner keeps the user at the ground level (xz plane)
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void rdCamera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	if (Yaw > 360) Yaw -= 360;
	if (Yaw < 0) Yaw += 360;

	// Update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void rdCamera::processMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

//初始方向(0,1,0)
void rdCamera::updateCameraVectors()
{
	// Calculate the new Front vector
	QVector3D front;
	/*front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));*/
	float yawRadian = qDegreesToRadians(Yaw);
	float pitchRadian = qDegreesToRadians(Pitch);
	float x = sin(yawRadian) * cos(pitchRadian);
	//front.x = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.setX(x);
	float y = cos(yawRadian) * cos(pitchRadian);
	//front.y = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.setY(y);

	float z = sin(pitchRadian);
	//front.z = sin(glm::radians(Pitch));
	front.setZ(z);
	//Front = glm::normalize(front);
	front.normalize();
	Front = front;

	// Also re-calculate the Right and Up vector
	QVector3D cc = QVector3D::crossProduct(Front, WorldUp);
	cc.normalize();
	Right = cc;// glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	cc = QVector3D::crossProduct(Right, Front);
	cc.normalize();
	Up = cc;// glm::normalize(glm::cross(Right, Front));
}

void rdCamera::setPos(float posX, float posY, float posZ)
{
	Position.setX(posX);
	Position.setY(posY);
	Position.setZ(posZ);
}

void rdCamera::setPos(QVector3D pos)
{
	Position = pos;
}

void rdCamera::addPosZ(float offset)
{
	Position.setZ(Position.z()+ offset);
}

QVector3D rdCamera::getPos()
{
	return QVector3D(Position.x(), Position.y(), Position.z());
}

void rdCamera::setYaw(float fYaw)
{
	Yaw = fYaw;
	updateCameraVectors();
}

void rdCamera::setPitch(float fPitch)
{
	Pitch = fPitch;
}

float rdCamera::getPitch()
{
	return Pitch;
}

float rdCamera::getYaw()
{
	return Yaw;
}