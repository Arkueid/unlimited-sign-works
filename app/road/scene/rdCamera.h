#ifndef RDCAMERA_H
#define RDCAMERA_H

#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = 0.0f;//偏航角
const float PITCH =0.0f; //俯仰角
const float SPEED = 2.5f;
const float SENSITIVITY = 0.002f;
const float ZOOM = 45.0f;

class rdCamera
{
public:
	~rdCamera();

	// Camera Attributes
	QVector3D Position;
	QVector3D Front;
	QVector3D Up;
	QVector3D Right;
	QVector3D WorldUp;
	
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	QMatrix4x4 m_mProjection;

public:
	// Constructor with vectors
	rdCamera(QVector3D position = QVector3D(0.0f, 0.0f, 13.25f/*0.0f, 0.0f, 0.0f*/), 
		     QVector3D up = QVector3D(0.0f, 0.0f, 1.0f),float yaw = YAW, float pitch = PITCH) :  
		     Front(QVector3D(0.0f, 1.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();

		m_mProjection.perspective(60.0f, 1.5f, 0.3f, 30000.0f);

		m_offsetZ = 10;
	}
	// Constructor with scalar values
	rdCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(QVector3D(0.0f, 1.0f, 0.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = QVector3D(posX, posY, posZ);
		WorldUp = QVector3D(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();

		m_mProjection.setToIdentity();
		m_mProjection.perspective(60.0f/*glm::radians()*/, 1.5f/*(float)cx / cy*/, 0.3f, 30000.0f);
	}

	QMatrix4x4 getProjectionMatrix();
	void setProjectionMatrix(QMatrix4x4 matrix);
	QMatrix4x4 getViewMatrix();
	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyboard(Camera_Movement direction, float deltaTime);
	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void processMouseScroll(float yoffset);
	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

	void setPos(float posX, float posY, float posZ);
	void setPos(QVector3D pos);
	QVector3D getPos();

	void setYaw(float fYaw);
	void setPitch(float fPitch);
	float getPitch();
	float getYaw();
	float getOffsetZ();
	void setOffsetZ(float z);
	void addPosZ(float offset);
private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors();	

	float m_offsetZ;//相机抬升高度
	// Euler Angles
	float Yaw;
	float Pitch;
};

#endif // RDCAMERA_H