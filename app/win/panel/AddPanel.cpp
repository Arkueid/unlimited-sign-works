#include <win/panel/AddPanel.hpp>
#include <base/Cube.hpp>
#include <base/Cylinder.hpp>
#include <base/Stiffener.hpp>
#include <base/RoundedCube.hpp>
#include <base/RoundedRegTriPrism.hpp>

#include <QMessageBox>

AddPanel::AddPanel(PoleScene *scene, QWidget *parent) : scene(scene),
                                                        ui(new Ui::AddPanel),
                                                        doubleValidator(new QDoubleValidator)
{
    ui->setupUi(this);

    initializeLineEdits();

    setAttribute(Qt::WA_DeleteOnClose);

    ui->panels->setCurrentIndex(0);

    connect(ui->addBtn, &QPushButton::clicked, this, &AddPanel::addPart);
}

AddPanel::~AddPanel()
{
    // qDebug() << "AddPanel destroyed";
    delete ui;
}

void AddPanel::addPart()
{
    switch (ui->panels->currentIndex()) {
        case 0:
            addCube();
            break;
        case 1:
            addCylinder();
            break;
        case 2:
            addStiffener();
            break;
        case 3:
            addRoundedCube();
            break;
        case 4:
            addRoundedRegTriPrism();
            break;
        default:
            break;
    }
}

void AddPanel::initializeLineEdits()
{
    doubleValidator->setRange(-1000000000, 1000000000, 5);
	doubleValidator->setNotation(QDoubleValidator::StandardNotation);

	ui->cubeA->setValidator(doubleValidator);
	ui->cubeB->setValidator(doubleValidator);
	ui->cubeC->setValidator(doubleValidator);

	ui->cylinderD->setValidator(doubleValidator);
	ui->cylinderH->setValidator(doubleValidator);

	ui->rCubeA->setValidator(doubleValidator);
	ui->rCubeB->setValidator(doubleValidator);
	ui->rCubeC->setValidator(doubleValidator);
	ui->rCubeRadius->setValidator(doubleValidator);

	ui->rRegTriPrismA->setValidator(doubleValidator);
	ui->rRegTriPrismH->setValidator(doubleValidator);
	ui->rRegTriPrismRadius->setValidator(doubleValidator);

	ui->stiffenerA->setValidator(doubleValidator);
	ui->stiffenerB->setValidator(doubleValidator);
	ui->stiffenerC->setValidator(doubleValidator);
	ui->stiffenerH1->setValidator(doubleValidator);
	ui->stiffenerH2->setValidator(doubleValidator);
	ui->stiffenerOffsetH->setValidator(doubleValidator);
	ui->stiffenerOffsetV->setValidator(doubleValidator);
}

bool AddPanel::isParamsValid(const QVector<QLineEdit*>& numbers, const QVector<QLineEdit*>& texts)
{
    for (auto it : numbers)
    {
        if (it->text().toFloat() <= 0)
        {
            QMessageBox::warning(this, "添加部件", "参数不能为空或负数", QMessageBox::Ok);
            return false;
        }
    }
    for (auto it : texts)
    {
        if (it->text().isEmpty())
        {
            QMessageBox::warning(this, "添加部件", "参数不能为空！", QMessageBox::Ok);
            return false;
        }
    }
    return true;
}

void AddPanel::addCube()
{
    if (!isParamsValid({ui->cubeA, ui->cubeB, ui->cubeC}, {ui->cubeName}))
    {
        return;
    }

    const float a = ui->cubeA->text().toFloat();
    const float b = ui->cubeB->text().toFloat();
    const float c = ui->cubeC->text().toFloat();
    QString name = ui->cubeName->text();
    
    Cube* cube = Cube::create(a, b, c);
    Part* part = new Part(cube, name.toStdString(), { 0, 0, 0 }, { 0, 0, 0 }, { -1, -1, -1, -1, -1, -1 });
    scene->addDrawable(part);

    close();
}

void AddPanel::addCylinder()
{
    if (!isParamsValid({ ui->cylinderD, ui->cylinderH }, { ui->cylinderName }))
    {
        return;
    }

    const float d = ui->cylinderD->text().toFloat();
    const float h = ui->cylinderH->text().toFloat();
    QString name = ui->cylinderName->text();
    Cylinder* cylinder = Cylinder::create(d, h);
    Part* part = new Part(cylinder, name.toStdString(), { 0, 0, 0 }, { 0, 0, 0 }, { -1, -1, -1 });
    scene->addDrawable(part);

    close();
}

void AddPanel::addStiffener()
{
    if (!isParamsValid({ ui->stiffenerA, ui->stiffenerB, ui->stiffenerC, ui->stiffenerH1, ui->stiffenerH2, ui->stiffenerOffsetH, ui->stiffenerOffsetV }, {ui->stiffenerName}))
    {
        return;
    }

    const float a = ui->stiffenerA->text().toFloat();
    const float b = ui->stiffenerB->text().toFloat();
    const float c = ui->stiffenerC->text().toFloat();
    const float h1 = ui->stiffenerH1->text().toFloat();
    const float h2 = ui->stiffenerH2->text().toFloat();
    const float offsetH = ui->stiffenerOffsetH->text().toFloat();
    const float offsetV = ui->stiffenerOffsetV->text().toFloat();
    QString name = ui->stiffenerName->text();

    Stiffener* stiffener = Stiffener::create(a, b, h1, h2, c, offsetH, offsetV);
    Part* part = new Part(stiffener, name.toStdString(), { 0, 0, 0 }, { 0, 0, 0 }, { -1, -1, -1, -1, -1, -1, -1 });
    scene->addDrawable(part);

    close();
}

void AddPanel::addRoundedCube()
{
    if (!isParamsValid({ ui->rCubeA, ui->rCubeB, ui->rCubeC, ui->rCubeRadius }, { ui->rCubeName }))
    {
        return;
    }

    const float a = ui->rCubeA->text().toFloat();
    const float b = ui->rCubeB->text().toFloat();
    const float c = ui->rCubeC->text().toFloat();
    const float radius = ui->rCubeRadius->text().toFloat();
    QString name = ui->rCubeName->text();

    RoundedCube* cube = RoundedCube::create(a, b, c, radius);
    Part* part = new Part(cube, name.toStdString(), { 0,0,0 }, { 0,0,0 }, { -1, -1, -1 });
    scene->addDrawable(part);

    close();
}

void AddPanel::addRoundedRegTriPrism()
{
    if (!isParamsValid({ ui->rRegTriPrismA, ui->rRegTriPrismH, ui->rRegTriPrismRadius }, { ui->rRegTriPrismName }))
    {
        return;
    }

    const float a = ui->rRegTriPrismA->text().toFloat();
    const float h = ui->rRegTriPrismH->text().toFloat();
    const float r = ui->rRegTriPrismRadius->text().toFloat();
    QString name = ui->rRegTriPrismName->text();

    RoundedRegTriPrism* rrtp = RoundedRegTriPrism::create(a, h, r);
    Part* part = new Part(rrtp, name.toStdString(), { 0, 0, 0 }, { 0, 0, 0 }, { -1, -1, -1 });
    scene->addDrawable(part);

    close();
}
