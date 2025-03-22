#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QMatrix4x4>

class Camera
{
public:
    Camera(const QVector3D &position, const QVector3D &up, float yaw, float pitch);

    void processKeyboard(int direction, float deltaTime); // 处理键盘输入
    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true); // 处理鼠标移动
    void processKeyboardForVerticalMovement(int direction, float deltaTime);
    QMatrix4x4 getViewMatrix(); // 获取视图矩阵

private:
    QVector3D position;
    QVector3D front;
    QVector3D up;
    QVector3D right;
    QVector3D worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;

    void updateCameraVectors(); // 更新摄像机向量
};

#endif // CAMERA_H