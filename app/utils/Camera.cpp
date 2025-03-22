#include <utils/Camera.hpp>
#include "Camera.hpp"

Camera::Camera(const QVector3D &position, const QVector3D &up, float yaw, float pitch)
    : front(QVector3D(0.0f, 0.0f, -1.0f)), movementSpeed(2.5f), mouseSensitivity(0.1f),
      position(position), worldUp(up), yaw(yaw), pitch(pitch)
{
    this->updateCameraVectors();
}

void Camera::processKeyboard(int direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction & 1)
        position += front * velocity; // 前进
    if (direction & 2)
        position -= front * velocity; // 后退
    if (direction & 4)
        position -= right * velocity; // 左移
    if (direction & 8)
        position += right * velocity; // 右移
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    // 确保不会发生翻转
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    this->updateCameraVectors();
}

void Camera::processKeyboardForVerticalMovement(int direction, float deltaTime)
{
    float velocity = movementSpeed * deltaTime;
    if (direction == 1) // 向上移动
        position += worldUp * velocity;
    else if (direction == -1) // 向下移动
        position -= worldUp * velocity;
}

QMatrix4x4 Camera::getViewMatrix()
{
    QMatrix4x4 view;
    view.lookAt(position, position + front, up);
    return view;
}

void Camera::updateCameraVectors()
{
    QVector3D frontVec;
    frontVec.setX(qCos(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
    frontVec.setY(qSin(qDegreesToRadians(pitch)));
    frontVec.setZ(qSin(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
    front = frontVec.normalized();

    right = QVector3D::crossProduct(front, worldUp).normalized();
    up = QVector3D::crossProduct(right, front).normalized();
}