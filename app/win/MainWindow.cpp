#include "MainWindow.hpp"
#include <win/PoleDesigner.hpp>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    resize(1280, 720);

    ui->statusbar->showMessage("I am the bone of my sword. "
                               "So as I pray, unlimited blade works. ");

    auto newFunc = [=]()
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
    };
    connect(ui->actionNew, &QAction::triggered, newFunc);
    connect(ui->clbNew, &QCommandLinkButton::clicked, newFunc);

    auto addFunc = [=]()
    {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        "Open Pole Json",
                                                        QDir::currentPath(),
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
    };

    connect(ui->actionOpen, &QAction::triggered, addFunc);
    connect(ui->clbOpen, &QCommandLinkButton::clicked, addFunc);

    connect(ui->tabs, &QTabWidget::tabCloseRequested,
            [=](int index)
            {
                QWidget* widget = ui->tabs->widget(index);
                // qDebug() << "Closing tab " << index;
                ui->tabs->removeTab(index);
                widget->deleteLater();
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}