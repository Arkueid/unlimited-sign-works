#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <utils/Camera.hpp>
#include <part/pole/Pole168.hpp>
#include <QVector>
#include <render/GLObject.hpp>

class GLScene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

    QOpenGLShaderProgram program;
    Camera camera;
    float lastFrame;
    float deltaTime;

    Pole168 pole;

    QVector<GLObject> drawables;

    QMatrix4x4 projection;

    bool firstMouse = true;
    float lastX;
    float lastY;
public:
    GLScene();
    ~GLScene() override;
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override; // 添加鼠标移动事件处理器
};