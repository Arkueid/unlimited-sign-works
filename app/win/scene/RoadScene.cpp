#include "RoadScene.hpp"

#include <QMouseEvent>

#include <utils/TextureManager.hpp>

#include <base/Cube.hpp>
#include <base/Cylinder.hpp>

RoadScene::RoadScene(QWidget *parent) : QOpenGLWidget(parent),
                                        camera(QVector3D(0.0f, 0.0f, 10.0f), QVector3D(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
                                        firstMouse(true),
                                        deltaTime(1.0f / 120.0f),
                                        drawables(0), roadScene(nullptr)
{
    setFocusPolicy(Qt::ClickFocus);

    camera.movementSpeed = 1000.0f;
}

RoadScene::~RoadScene()
{
    makeCurrent();
    releaseDrawables();
    program.removeAllShaders();
    TextureManager::releaseTextures();

    delete roadScene;

    doneCurrent();
}

PoleDrawable *RoadScene::addPole(Pole *pole, float peg, bool isLeft)
{
    makeCurrent();

    PoleDrawable *d = placePole(pole, peg, isLeft);

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

    initRoad();

    Point &point = roadScene->m_originPoint;
    origin = {point.y, camera.position.z(), point.x};
    camera.position = {origin.x, origin.y, origin.z};
    camera.yaw = roadScene->m_originYaw;
    camera.updateCameraVectors();

    emit initialized();
}

void RoadScene::paintGL()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 view = camera.getViewMatrix();

    roadScene->render(projection, view);

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
    float fov = 60.0f;                       // 视野角度
    float aspectRatio = (float)w / (float)h; // 窗口比例
    float nearPlane = 0.1f;                  // 近裁剪面
    float farPlane = 30000.0f;               // 远裁剪面

    projection.setToIdentity();
    projection.perspective(fov, aspectRatio, nearPlane, farPlane);

    projectionSky.ortho(-1, 1, -1, 1, -20, 20);

    roadScene->m_pSky->setProjection(projectionSky);
}

void RoadScene::keyPressEvent(QKeyEvent *event)
{
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

void RoadScene::initRoad()
{
    roadScene = new rdScene;

    roadScene->loadTextures();
    roadScene->loadShaders();
    roadScene->loadData();
    roadScene->computing();

    roadScene->initGeometry();
}

PoleDrawable *RoadScene::placePole(Pole *pole, float peg, bool isLeft)
{
    PoleDrawable *d = new PoleDrawable(pole);

    _PosAng p = roadScene->m_rl.findPos(peg);
    // qDebug() << peg;
    // qDebug() << p.y << p.z << p.x << p.angle << p.section;

    _RoadCrossSection sec = roadScene->m_rl.getRCS(p.section, 1);
    _RoadCrossSection secNext = roadScene->m_rl.getRCS(p.section + 1, 1);

    const float t = (peg - sec.pileNo) / (secNext.pileNo - sec.pileNo);

    // qDebug() << t;

    QMatrix4x4 model;
    model.setToIdentity();

    // 左道，朝向逆方向车辆
    const float faceAngle = isLeft ? 90 : -90;
    const float angle = 360 - p.angle + faceAngle;

    const float radius = angle / 180.0f * PI;

    // qDebug() << angle;

    const float sideOffset = 0.5f;

    const QVector3D &center1 = sec.center;
    const QVector3D &center2 = secNext.center;
    const QVector3D &center = (center2 - center1) * t + center1;

    if (pole->getType() == PoleType_DoubleArm) // 摆在路中央：双悬臂、门架
    {
        model.translate(center.y(), center.z() - 0.01, center.x());
    }
    else if (pole->getType() == PoleType_Gate) // 门架并非完全摆在路中间
    {
        const float leftSize1 = sec.lW1 + sec.lW2 + sec.lW3 + sec.W0 / 2;
        const float rightSize1 = sec.rW1 + sec.rW2 + sec.rW3 + sec.W0 / 2;
        const float leftSize2 = secNext.lW1 + secNext.lW2 + secNext.lW3 + secNext.W0 / 2;
        const float rightSize2 = secNext.rW1 + secNext.rW2 + secNext.rW3 + secNext.W0 / 2;
        const float leftSize = (leftSize2 - leftSize1) * t + leftSize1;
        const float rightSize = (rightSize2 - rightSize1) * t + rightSize1;

        const float leftRadius = (360 - p.angle + 90) / 180.0f * PI;
        const float rightRadius = (360 - p.angle - 90) / 180.0f * PI;
        const float leftX = leftSize * cos(leftRadius) + center.y();
        const float leftZ = -leftSize * sin(leftRadius) + center.x();

        const float rightX = rightSize * cos(rightRadius) + center.y();
        const float rightZ = -rightSize * sin(rightRadius) + center.x();

        const float x = (leftX + rightX) / 2.0f;
        const float z = (leftZ + rightZ) / 2.0f;
        qDebug() << x << z;
        qDebug() << center.y() << center.x();
        model.translate(x, center.z() - 0.01, z);

        // 适应路面宽度
        QVector<QVector2D> points(2);
        int index = 0;
        // 获取底座水平面坐标，用于计算两个立柱水平距离
        for (auto &part : pole->getParts())
        {
            if (part->isPoleBase())
            {
                points[index++] = {part->getOriginX(), part->getOriginZ()};
                if (index >= 2)
                {
                    break;
                }
            }
        }
        // 基底最大为1500mm
        const float scale = (leftSize + rightSize + 1.5f) * 1000 / (points[0] - points[1]).length();
        // qDebug() << scale;
        model.scale(scale);
    }
    // 摆在路侧：单柱、双柱、单悬臂
    // 路左：左为相机初始时朝向时，右车道行驶的方向
    else
    {
        const float size1 = isLeft ? (sec.lW1 + sec.lW2 + sec.lW3 + sec.W0 / 2) : (sec.rW1 + sec.rW2 + sec.rW3 + sec.W0 / 2);
        const float size2 = isLeft ? (secNext.lW1 + secNext.lW2 + secNext.lW3 + secNext.W0 / 2) : (secNext.rW1 + secNext.rW2 + secNext.rW3 + secNext.W0 / 2);
        const float size = (size2 - size1) * t + size1;

        float r = sideOffset + size;

        // 双柱需要考虑其宽度
        if (pole->getType() == PoleType_Double)
        {
            QVector<QVector2D> points(2);
            int index = 0;
            for (auto &part : pole->getParts())
            {
                if (part->isPoleBase())
                {
                    points[index++] = {part->getOriginX(), part->getOriginZ()};
                    if (index >= 2)
                    {
                        break;
                    }
                }
            }

            // 两立柱水平距离一半
            const float halfPoleDistance = (points[0] - points[1]).length() / 2.0f / 1000;
            r += halfPoleDistance;
        }

        model.translate(center.y() + cos(radius) * r, center.z() - 0.01, center.x() - sin(radius) * r);
    }

    model.rotate(angle, {0.0f, 1.0f, 0.0f});

    d->setModelMatrix(model);

    drawables.append(d);
    return d;
}

const QVector<PoleDrawable *> RoadScene::getDrawables() const
{
    return drawables;
}
