#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

#include <utils/Camera.hpp>
#include <QVector>
#include <render/PartDrawable.hpp>
#include <pole/Pole.hpp>

#include <render/AxisDrawable.hpp>
#include <render/BoxDrawable.hpp>

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

    AxisDrawable* axis;
    BoxDrawable* wrapBox;

    bool partSelected;

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

    void loadPole(const QString& poleJsonPath);

    void selectPart(PartDrawable* drawable);

    void unselectPart();

    int getDrawableCount() const
    {
        drawables.size();
    }

    PartDrawable* getDrawable(int index);
    void removeDrawable(PartDrawable* drawable);
    void addDrawable(Part* part);
    void updatePart(PartDrawable* drawable);
    void unbindPart(PartDrawable* drawable);

    const QVector<PartDrawable*>& getDrawables() const
    {
        return drawables;
    }

    QString getPoleName()
    {
        return pole->getName().c_str();
    }

    void setPoleName(const QString& name)
    {
        pole->setName(name.toStdString());
    }

    Pole* getPole()
    {
        return pole;
    }

    void changePole(Pole* pole);

signals:
    void drawableAdded(PartDrawable* drawable);

    void initialized();
};