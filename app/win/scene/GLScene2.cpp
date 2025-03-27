#include <win/scene/GLScene2.hpp>

#include <QTime>
#include <QKeyEvent>

GLScene2::GLScene2() : camera(QVector3D(0.0f, 0.0f, 1.0f), QVector3D(0.0f, 1.0f, 0.0f), -90.0f, 0.0f),
                     deltaTime(0.0f)
{
}

GLScene2::~GLScene2()
{
    releaseDrawables();
}

void GLScene2::releaseDrawables()
{
    for (auto& d : drawables)
    {
        delete d;
    }
    drawables.clear();
}

void GLScene2::initializeGL()
{
    initializeOpenGLFunctions();

    program.addShaderFromSourceFile(QOpenGLShader::Vertex, "../../assets/shaders/vshader.glsl");
    program.addShaderFromSourceFile(QOpenGLShader::Fragment, "../../assets/shaders/fshader.glsl");
    program.link();

    if (!program.isLinked())
    {
        qDebug() << "Shader program is not linked\n";
    }

    for (auto& part : pole168.getParts())
    {
        drawables.append(new Drawable(&part));
    }

    pole89DrawableOffset = drawables.size();
    for (auto& part : pole89.getParts())
    {
        drawables.append(new Drawable(&part));
    }

    deltaTime = 1.0f / 60.0f;
}

void GLScene2::paintGL()
{

    glEnable(GL_DEPTH_TEST);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.bind();
    program.setUniformValue("projection", projection);
    program.setUniformValue("view", camera.getViewMatrix());

    QMatrix4x4 transform168;
    transform168.setToIdentity();

    QMatrix4x4 transform89;
    transform89.setToIdentity();
    transform89.translate(-0.30f, 0.0f, 0.0f);
    int idx = 0;
    for (auto& d : drawables)
    {
        if (idx < pole89DrawableOffset)
        {
            d->draw(program, transform168);
        }
        else
        {
            d->draw(program, transform89);
        }
        idx++;
    }

    program.release();

    // qDebug() << "updated";
}

void GLScene2::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    float fov = 45.0f;                       // 视野角度
    float aspectRatio = (float)w / (float)h; // 假设窗口大小为800x600
    float nearPlane = 0.1f;                  // 近裁剪面
    float farPlane = 100.0f;                 // 远裁剪面

    projection.setToIdentity();
    projection.perspective(fov, aspectRatio, nearPlane, farPlane);
}

void GLScene2::keyPressEvent(QKeyEvent *event)
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

void GLScene2::mousePressEvent(QMouseEvent *event)
{
    lastX = event->x();
    lastY = event->y();
}

void GLScene2::mouseMoveEvent(QMouseEvent *event)
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
