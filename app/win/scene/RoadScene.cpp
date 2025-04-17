#include "RoadScene.hpp"

#include <QMouseEvent>

#include <utils/TextureManager.hpp>

RoadScene::RoadScene(QWidget *parent) : QOpenGLWidget(parent),
                                        camera(QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
                                        firstMouse(true),
                                        deltaTime(1.0f / 120.0f),
                                        drawables(0)
{
    setFocusPolicy(Qt::ClickFocus);
}

RoadScene::~RoadScene()
{
    makeCurrent();
    releaseDrawables();
    program.removeAllShaders();
    TextureManager::releaseTextures();
    
    doneCurrent();
}

PoleDrawable* RoadScene::addPole(Pole *pole)
{
    makeCurrent();
    PoleDrawable* d = new PoleDrawable(pole);
    drawables.append(d);
    doneCurrent();

    update();
    return d;
}

void RoadScene::removePole(PoleDrawable *drawable)
{
    makeCurrent();
    drawables.removeOne(drawable);
    delete drawable;

    doneCurrent();

    update();
}

PoleDrawable *RoadScene::getPoleDrawable(int index)
{
    return drawables[index];
}

void RoadScene::initializeGL()
{
    initializeOpenGLFunctions();

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "../../assets/shaders/vshader.glsl");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "../../assets/shaders/fshader.glsl");
    program.link();

    if (!program.isLinked())
    {
        qDebug() << "Shader program is not linked\n";
    }
}

void RoadScene::paintGL()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view = camera.getViewMatrix();
    axis.draw(projection, view);

    program.bind();
    program.setUniformValue("projection", projection);
    program.setUniformValue("view", view);
    for (int i = 0; i < drawables.size(); i++)
    {
        if (drawables[i]->isVisible())
        {
            drawables[i]->draw(program);
        }
    }
    program.release();
}

void RoadScene::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    float fov = 45.0f;                       // 视野角度
    float aspectRatio = (float)w / (float)h; // 窗口比例
    float nearPlane = 0.1f;                  // 近裁剪面
    float farPlane = 100.0f;                 // 远裁剪面

    projection.setToIdentity();
    projection.perspective(fov, aspectRatio, nearPlane, farPlane);
}

void RoadScene::keyPressEvent(QKeyEvent *event)
{
    float cameraSpeed = 2.5f * deltaTime; // 控制相机移动速度
    if (event->key() == Qt::Key_W)
    {
        camera.processKeyboard(1, deltaTime); // 前进
    }
    else if (event->key() == Qt::Key_S)
    {
        camera.processKeyboard(2, deltaTime); // 后退
    }
    else if (event->key() == Qt::Key_A)
    {
        camera.processKeyboard(4, deltaTime); // 左移
    }
    else if (event->key() == Qt::Key_D)
    {
        camera.processKeyboard(8, deltaTime); // 右移
    }
    else if (event->key() == Qt::Key_Q)
    { // 上升
        camera.processKeyboardForVerticalMovement(1, deltaTime);
    }
    else if (event->key() == Qt::Key_E)
    { // 下降
        camera.processKeyboardForVerticalMovement(-1, deltaTime);
    }

    update();
}

void RoadScene::mousePressEvent(QMouseEvent *event)
{
    lastX = event->x();
    lastY = event->y();
}

void RoadScene::mouseMoveEvent(QMouseEvent *event)
{
    if (firstMouse)
    {
        lastX = event->x();
        lastY = event->y();
        firstMouse = false;
    }

    float xoffset = event->x() - lastX;
    float yoffset = lastY - event->y(); // 注意这里是反向的，因为Y坐标是从顶部开始计算的
    lastX = event->x();
    lastY = event->y();

    camera.processMouseMovement(xoffset, yoffset);
    update();
}

void RoadScene::focusInEvent(QFocusEvent *event)
{
    grabKeyboard();
}

void RoadScene::focusOutEvent(QFocusEvent *event)
{
    releaseKeyboard();
}

void RoadScene::releaseDrawables()
{
    qDeleteAll(drawables);
    drawables.clear();
}
