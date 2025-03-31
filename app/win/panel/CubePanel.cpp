#include <win/panel/CubePanel.hpp>
#include <base/Cube.hpp>

#include <utils/TextureManager.hpp>

CubePanel::CubePanel(PoleScene *scene,
                               QWidget *parent) : QWidget(parent),
                                                  ui(new Ui::CubePanel),
                                                  scene(scene)

{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->submitBtn, &QPushButton::clicked, this, &CubePanel::onSubmit);
}

bool CubePanel::isParamValid()
{
    return true;
}

CubePanel::~CubePanel()
{
    delete ui;
}

void CubePanel::onSubmit()
{
    if (!isParamValid())
        return;

    Mesh *mesh = new Cube(ui->a->text().toFloat(), ui->b->text().toFloat(), ui->c->text().toFloat());

    Part *part = new Part(mesh, ui->name->text().toStdString(),
                          {
                              ui->originX->text().toFloat(),
                              ui->originY->text().toFloat(),
                              ui->originZ->text().toFloat(),
                          },
                          {
                              ui->rotationX->text().toFloat(),
                              ui->rotationY->text().toFloat(),
                              ui->rotationZ->text().toFloat(),
                          },
                          {0, 1, 1, 1, 1, 1},
                          TextureManager::getTexturePaths());

    scene->addDrawable(part);
    // qDebug() << "CubeParamPanel::onSubmit()";
    close();
}