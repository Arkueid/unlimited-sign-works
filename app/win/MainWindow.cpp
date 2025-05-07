#include <win/MainWindow.hpp>

#include <QFileDialog>

#include <win/texture/BoardDesigner.hpp>
#include <win/texture/TextureBase.hpp>
#include <win/pole/PoleDesigner.hpp>
#include <win/road/RoadDesigner.hpp>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(1280, 720);

    ui->statusbar->showMessage("I am the bone of highway traffic sign. "
                               "So as I pray, unlimited sign works. ");

    connect(ui->actionNewPole, &QAction::triggered, this, &MainWindow::onNewPoleRequested);

    connect(ui->actionOpenPole, &QAction::triggered, this, &MainWindow::onOpenPoleRequested);

    connect(ui->actionTexGen, &QAction::triggered, this, &MainWindow::onTexGenRequested);

    connect(ui->actionTexBase, &QAction::triggered, this, &MainWindow::onTexBaseRequested);

    connect(ui->tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabClosed);

    connect(ui->actionNewRoad, &QAction::triggered, this, &MainWindow::onNewRoadRequested);
    connect(ui->clbNewRoad, &QCommandLinkButton::clicked, this, &MainWindow::onNewRoadRequested);

    connect(ui->actionOpenRoad, &QAction::triggered, this, &MainWindow::onOpenRoadRequested);
    connect(ui->clbOpenRoad, &QCommandLinkButton::clicked, this, &MainWindow::onOpenRoadRequested);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewRoadRequested()
{
    RoadDesigner *designer = new RoadDesigner(ui->tabs);
    designer->getRoadNameLineEdit()->setText("New Road");
    const int index = ui->tabs->addTab(designer, designer->getRoadNameLineEdit()->text());
    connect(designer->getRoadNameLineEdit(), &QLineEdit::textChanged,
            [=](const QString &name)
            {
                ui->tabs->setTabText(index, name);
            });
    ui->tabs->setCurrentIndex(index);
}

void MainWindow::onOpenRoadRequested()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open Road Json",
                                                    QDir::currentPath() + "/../../assets/roads",
                                                    "Road Json (*.json)");
    if (fileName.isEmpty())
    {
        return;
    }

    RoadDesigner *designer = new RoadDesigner(ui->tabs, fileName);
    const int index = ui->tabs->addTab(designer, "Open Road");
    connect(designer->getRoadNameLineEdit(), &QLineEdit::textChanged,
            [=](const QString &name)
            {
                ui->tabs->setTabText(index, name);
            });
    ui->tabs->setCurrentIndex(index);
}

void MainWindow::onNewPoleRequested()
{
    PoleDesigner *designer = new PoleDesigner(ui->tabs);
    designer->getPoleNameLineEdit()->setText("New Pole");
    const int index = ui->tabs->addTab(designer, designer->getPoleNameLineEdit()->text());
    connect(designer->getPoleNameLineEdit(), &QLineEdit::textChanged,
            [=](const QString &name)
            {
                ui->tabs->setTabText(index, name);
            });
    ui->tabs->setCurrentIndex(index);
}

void MainWindow::onOpenPoleRequested()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open Pole Json",
                                                    QDir::currentPath() + "/../../assets/poles",
                                                    "Pole Json (*.json)");
    if (fileName.isEmpty())
    {
        return;
    }

    PoleDesigner *designer = new PoleDesigner(ui->tabs, fileName);
    int index = ui->tabs->addTab(designer, designer->getPoleNameLineEdit()->text());
    connect(designer->getPoleNameLineEdit(), &QLineEdit::textChanged,
            [=](const QString &name)
            {
                ui->tabs->setTabText(index, name);
            });
    ui->tabs->setCurrentIndex(index);
}

void MainWindow::onTexGenRequested()
{
    for (int i = 0; i < ui->tabs->count(); ++i)
    {
        if (ui->tabs->widget(i)->metaObject()->className() == QString("BoardDesigner"))
        {
            ui->tabs->setCurrentIndex(i);
            return;
        }
    }
    BoardDesigner *designer = new BoardDesigner(ui->tabs);
    const int index = ui->tabs->addTab(designer, "纹理生成");
    ui->tabs->setCurrentIndex(index);
}

void MainWindow::onTexBaseRequested()
{
    for (int i = 0; i < ui->tabs->count(); ++i)
    {
        if (ui->tabs->widget(i)->metaObject()->className() == QString("TextureBase"))
        {
            ui->tabs->setCurrentIndex(i);
            return;
        }
    }
    TextureBase *base = new TextureBase(ui->tabs);
    const int index = ui->tabs->addTab(base, "纹理库");
    ui->tabs->setCurrentIndex(index);
}

void MainWindow::onTabClosed(int index)
{
    QWidget *widget = ui->tabs->widget(index);
    // qDebug() << "Closing tab " << index;
    ui->tabs->removeTab(index);
    widget->deleteLater();
}