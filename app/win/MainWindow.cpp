#include <win/MainWindow.hpp>

#include <QFileDialog>

#include <win/texture/BoardDesigner.hpp>
#include <win/texture/TextureBase.hpp>
#include <win/pole/PoleDesigner.hpp>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(1280, 720);

    ui->statusbar->showMessage("I am the bone of highway traffic sign. "
                               "So as I pray, unlimited sign works. ");

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewProjectRequested);
    connect(ui->clbNew, &QCommandLinkButton::clicked, this, &MainWindow::onNewProjectRequested);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenProjectRequested);
    connect(ui->clbOpen, &QCommandLinkButton::clicked, this, &MainWindow::onOpenProjectRequested);

    connect(ui->actionTexGen, &QAction::triggered, this, &MainWindow::onTexGenRequested);

    connect(ui->actionTexBase, &QAction::triggered, this, &MainWindow::onTexBaseRequested);

    connect(ui->tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabClosed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNewProjectRequested()
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

void MainWindow::onOpenProjectRequested()
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