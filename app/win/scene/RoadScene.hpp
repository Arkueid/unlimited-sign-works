#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <utils/Camera.hpp>
#include <render/AxisDrawable.hpp>
#include <render/BoxDrawable.hpp>
#include <render/PoleDrawable.hpp>

#include <road/scene/rdScene.h>

class RoadScene : public QOpenGLWidget, protected QOpenGLFunctions
{

    Q_OBJECT

    QOpenGLShaderProgram program;

    AxisDrawable axis;

    Camera camera;
    float deltaTime;
    bool firstMouse;
    float lastX;
    float lastY;

    QMatrix4x4 projection;
    QMatrix4x4 projectionSky;

    QVector<PoleDrawable *> drawables;

    rdScene *roadScene;

    Point origin;

signals:
    void initialized();

public:
    RoadScene(QWidget *parent = nullptr);
    ~RoadScene() override;

    PoleDrawable *addPole(Pole *pole, float peg, bool isLeft);

    void removePole(PoleDrawable *drawable);

    PoleDrawable *getPoleDrawable(int index);

    const QVector<PoleDrawable *> getDrawables() const;

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

private:
    void releaseDrawables();

    void initRoad();

    PoleDrawable *placePole(Pole *pole, float peg, bool isLeft);
};