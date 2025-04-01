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
                PoleDesigner *designer = new PoleDesigner(this);
                designer->getPoleNameLineEdit()->setText("New Pole");
                const int index = ui->tabs->addTab(designer, designer->getPoleNameLineEdit()->text());
                connect(designer->getPoleNameLineEdit(), &QLineEdit::textChanged,
                        [=](const QString &name)
                        {
                            ui->tabs->setTabText(index, name);
                        });
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

                PoleDesigner *designer = new PoleDesigner(this, fileName);
                int index = ui->tabs->addTab(designer, designer->getPoleNameLineEdit()->text());
                connect(designer->getPoleNameLineEdit(), &QLineEdit::textChanged,
                        [=](const QString &name)
                        {
                            ui->tabs->setTabText(index, name);
                        });
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