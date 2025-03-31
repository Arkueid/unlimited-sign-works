#include <win/scene/PoleScene.hpp>

#include <QTime>
#include <QKeyEvent>

#include <utils/TextureManager.hpp>
#include <utils/VertexBufferManager.hpp>

#include <pole/Pole168.hpp>
#include <pole/Pole89.hpp>

PoleScene::PoleScene(QWidget *parent) : QOpenGLWidget(parent), camera(QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
                                        deltaTime(0.0f), drawables(0)
{
    pole = new Pole168();
    // pole = new Pole89();
    setFocusPolicy(Qt::ClickFocus);
}

PoleScene::~PoleScene()
{
    // qDebug() << "PoleScene::~PoleScene";
    makeCurrent();
    releaseDrawables();
    delete axis;
    VertexBufferManager::releaseVertexBuffers();
    program.removeAllShaders();
    TextureManager::releaseTextures();
    delete pole;
    doneCurrent();
}

PartDrawable *PoleScene::getDrawable(int index)
{
    return drawables[index];
}

void PoleScene::removeDrawable(PartDrawable *drawable)
{
    drawables.removeOne(drawable);
    delete drawable;
    update();
}

void PoleScene::addDrawable(Part *part)
{
    pole->addPart(part);
    pole->addShape(part->getMesh());
    PartDrawable *drawable = new PartDrawable(part);
    drawables.append(drawable);
    emit drawableAdded(drawable);
    update();
}

void PoleScene::releaseDrawables()
{
    if (!drawables.isEmpty())
    {
        qDeleteAll(drawables);
    }
    drawables.clear();
}

void PoleScene::initializeGL()
{
    initializeOpenGLFunctions();
    axis = new AxisDrawable();

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "../../assets/shaders/vshader.glsl");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "../../assets/shaders/fshader.glsl");
    program.link();

    if (!program.isLinked())
    {
        qDebug() << "Shader program is not linked\n";
    }

    const std::vector<Part*>& parts = pole->getParts();
    for (auto part : parts)
    {
        PartDrawable *drawable = new PartDrawable(part);
        drawables.append(drawable);
        emit drawableAdded(drawable);
    }

    deltaTime = 1.0f / 60.0f;
}

void PoleScene::paintGL()
{
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view = camera.getViewMatrix();

    axis->draw(projection, view);

    program.bind();
    program.setUniformValue("projection", projection);
    program.setUniformValue("view", view);
    for (auto &d : drawables)
    {
        if (d->isVisible())
        {
            d->draw(program, transform);
        }
    }
    program.release();

    // qDebug() << "updated";
}

void PoleScene::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    float fov = 45.0f;                       // 视野角度
    float aspectRatio = (float)w / (float)h; // 假设窗口大小为800x600
    float nearPlane = 0.1f;                  // 近裁剪面
    float farPlane = 100.0f;                 // 远裁剪面

    projection.setToIdentity();
    projection.perspective(fov, aspectRatio, nearPlane, farPlane);
}

void PoleScene::keyPressEvent(QKeyEvent *event)
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

void PoleScene::mousePressEvent(QMouseEvent *event)
{
    lastX = event->x();
    lastY = event->y();
}

void PoleScene::mouseMoveEvent(QMouseEvent *event)
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

void PoleScene::focusInEvent(QFocusEvent *event)
{
    // qDebug() << "focusInEvent";
    grabKeyboard();
}

void PoleScene::focusOutEvent(QFocusEvent *event)
{
    // qDebug() << "focusOutEvent";
    releaseKeyboard();
}
