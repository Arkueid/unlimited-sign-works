#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <utils/Camera.hpp>
#include <QVector>
#include <render/PartDrawable.hpp>
#include <part/pole/Pole.hpp>

class PoleScene : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

    QOpenGLShaderProgram program;

    Camera camera;
    float deltaTime;
    bool firstMouse = true;
    float lastX;
    float lastY;

    QVector<PartDrawable*> drawables;

    QMatrix4x4 projection;
    QMatrix4x4 transform;

    Pole* pole;

    void releaseDrawables();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

public:
    explicit PoleScene(QWidget *parent = nullptr);
    ~PoleScene() override;

    int getDrawableCount() const
    {
        drawables.size();
    }

    PartDrawable* getDrawable(int index);
    void removeDrawable(PartDrawable* drawable);
    void addDrawable(Part* part);

    const QVector<PartDrawable*>& getDrawables() const
    {
        return drawables;
    }

signals:
    void drawableAdded(PartDrawable* drawable);
};