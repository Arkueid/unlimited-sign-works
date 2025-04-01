#include "MainWindow.hpp"
#include <win/PoleDesigner.hpp>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(1280, 720);

    ui->statusbar->showMessage("I am the bone of my sword. "
                               "So as I pray, unlimited blade works. ");

    connect(ui->actionNew, &QAction::triggered,
            [=]()
            {
                int index = ui->tabs->addTab(new PoleDesigner(this), "New Tab");
                ui->tabs->setCurrentIndex(index);
            });

    connect(ui->actionOpen, &QAction::triggered,
            [=]()
            {
                QString fileName = QFileDialog::getOpenFileName(this,
                                                                "Open Pole Json",
                                                                QDir::currentPath(),
                                                                "Pole Json (*.json)");
                if (fileName.isEmpty())
                {
                    return;
                }
                int index = ui->tabs->addTab(new PoleDesigner(this, fileName), "New Tab");
                ui->tabs->setCurrentIndex(index);
            });

    connect(ui->tabs, &QTabWidget::tabCloseRequested,
            [=](int index)
            {
                // qDebug() << "Closing tab " << index;
                ui->tabs->removeTab(index);
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}