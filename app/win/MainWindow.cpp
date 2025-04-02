#include <win/MainWindow.hpp>
#include <win/PoleDesigner.hpp>
#include <QFileDialog>
#include <win/texture/TextureEditor.hpp>
#include <win/texture/TextureBase.hpp>

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

    auto openFunc = [=]()
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
    };

    connect(ui->actionOpen, &QAction::triggered, openFunc);
    connect(ui->clbOpen, &QCommandLinkButton::clicked, openFunc);

    connect(ui->actionTexGen, &QAction::triggered,
            [=]()
            {
                TextureEditor *editor = new TextureEditor(ui->tabs);
                const int index = ui->tabs->addTab(editor, "纹理生成");
                ui->tabs->setCurrentIndex(index);
            });

    connect(ui->actionTexBase, &QAction::triggered,
            [=]()
            {
                TextureBase *base = new TextureBase(ui->tabs);
                const int index = ui->tabs->addTab(base, "纹理库");
                ui->tabs->setCurrentIndex(index);
            });

    connect(ui->tabs, &QTabWidget::tabCloseRequested,
            [=](int index)
            {
                QWidget *widget = ui->tabs->widget(index);
                // qDebug() << "Closing tab " << index;
                ui->tabs->removeTab(index);
                widget->deleteLater();
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}