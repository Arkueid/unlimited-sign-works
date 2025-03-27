#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <utils/Camera.hpp>
#include <part/pole/Pole168.hpp>
#include <part/pole/Pole89.hpp>
#include <QVector>
#include <render/Drawable.hpp>

class GLScene2 : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

    QOpenGLShaderProgram program;
    Camera camera;
    float deltaTime;

    QVector<Drawable*> drawables;

    QMatrix4x4 projection;

    bool firstMouse = true;
    float lastX;
    float lastY;

#pragma region hack properties
    Pole168 pole168;
    Pole89 pole89;
    int pole89DrawableOffset;
#pragma endregion


    void releaseDrawables();
public:
    GLScene2();
    ~GLScene2() override;
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override; // 添加鼠标移动事件处理器
};