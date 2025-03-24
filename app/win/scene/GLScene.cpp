#include <win/scene/GLScene.hpp>
#include "GLScene.hpp"

#include <QTime>
#include <QKeyEvent>

GLScene::GLScene() : camera(QVector3D(0.0f, 0.0f, 3.0f), QVector3D(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
                     deltaTime(0.0f), lastFrame(0.0f)
{
}

GLScene::~GLScene()
{
}

void GLScene::initializeGL()
{
    initializeOpenGLFunctions();

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "../../assets/shaders/vshader.glsl");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "../../assets/shaders/fshader.glsl");
    program.link();

    if (!program.isLinked())
    {
        qDebug() << "Shader program is not linked\n";
    }

    drawables.append({&pole.mainPole, &pole.mainPoleMat});

    drawables.append({&pole.sideBeam, &pole.sideBeam1Mat});
    drawables.append({&pole.sideBeam, &pole.sideBeam2Mat});

    drawables.append({&pole.panel, &pole.panelMat});
    drawables.append({&pole.bottom, &pole.bottomMat});

    drawables.append({&pole.sideStiffener, &pole.sideStiffener1Mat});
    drawables.append({&pole.sideStiffener, &pole.sideStiffener2Mat});
    drawables.append({&pole.sideStiffener, &pole.sideStiffener3Mat});
    drawables.append({&pole.sideStiffener, &pole.sideStiffener4Mat});

    drawables.append({&pole.flange, &pole.flange1Mat});
    drawables.append({&pole.flange, &pole.flange2Mat});

    drawables.append({&pole.sideStiffener, &pole.sideStiffener5Mat});
    drawables.append({&pole.sideStiffener, &pole.sideStiffener6Mat});
    drawables.append({&pole.sideStiffener, &pole.sideStiffener7Mat});
    drawables.append({&pole.sideStiffener, &pole.sideStiffener8Mat});

    drawables.append({&pole.bottomStiffener, &pole.bottomStiffener1Mat});
    drawables.append({&pole.bottomStiffener, &pole.bottomStiffener2Mat});
    drawables.append({&pole.bottomStiffener, &pole.bottomStiffener3Mat});
    drawables.append({&pole.bottomStiffener, &pole.bottomStiffener4Mat});

    startTimer(1000 / 30);
}

void GLScene::paintGL()
{
    float currentFrame = float(QTime::currentTime().msecsSinceStartOfDay()) / 1000.0f;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.bind();
    program.setUniformValue("projection", projection);
    program.setUniformValue("view", camera.getViewMatrix());
    for (auto &drawable : drawables)
    {
        drawable.draw(&program);
    }
    program.release();

    // qDebug() << "updated";
}

void GLScene::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    float fov = 45.0f;                       // 视野角度
    float aspectRatio = (float)w / (float)h; // 假设窗口大小为800x600
    float nearPlane = 0.1f;                  // 近裁剪面
    float farPlane = 100.0f;                 // 远裁剪面

    projection.setToIdentity();
    projection.perspective(fov, aspectRatio, nearPlane, farPlane);
}

void GLScene::keyPressEvent(QKeyEvent *event)
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
}

void GLScene::timerEvent(QTimerEvent *event)
{
    update();
}

void GLScene::mousePressEvent(QMouseEvent *event)
{
    lastX = event->x();
    lastY = event->y();
}

void GLScene::mouseMoveEvent(QMouseEvent *event)
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
}
