#include <win/texture/BoardDesigner.hpp>

#include <win/scene/BoardScene.hpp>
#include <QFileDialog>
#include <QInputDialog>

BoardDesigner::BoardDesigner(QWidget *parent) : QWidget(parent), ui(new Ui::BoardDesigner)
{
    ui->setupUi(this);

    initTemplateSelector();
    initValidators();

    connect(ui->scene, &BoardScene::selectionChanged, this, &BoardDesigner::onSelectionChanged);

    connect(ui->textInput, &QLineEdit::textEdited, this, &BoardDesigner::onTextEdited);

    connect(ui->exportBtn, &QPushButton::clicked, this, &BoardDesigner::onExportBtnClicked);
}

BoardDesigner::~BoardDesigner()
{
    delete ui;
}

void BoardDesigner::onExportBtnClicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter("Texture Image (*.png)");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    QDir dir = QDir::current();
    dir.cdUp();
    dir.cdUp();
    // ../../assets/poles
    dialog.setDirectory(dir.absoluteFilePath("assets/textures/generated"));
    dialog.selectFile("New Image.png");
    if (dialog.exec())
    {
        QString filePath = dialog.selectedFiles().first();
        // qDebug() << "Saving to: " << filePath;
        ui->scene->exportImage(filePath);
    }
}

void BoardDesigner::initTemplateSelector()
{
    QDir dir = QDir::current();
    dir.cdUp();
    dir.cdUp();
    dir.cd("assets/textures/template");
    QStringList fileNames = dir.entryList(QDir::Files);
    for (auto &fn : fileNames)
    {
        if (fn.endsWith(".cfg"))
        {
            ui->templateSelector->addItem(fn.split(".").first());
        }
    }

    ui->templateSelector->setCurrentIndex(-1);

    connect(ui->templateSelector, &QComboBox::currentTextChanged, this, &BoardDesigner::onTemplateSelected);
}

void BoardDesigner::onSelectionChanged(int index)
{
    if (index == -1)
    {
        ui->textInput->setText("");
        return;
    }
    ui->textInput->setText(ui->scene->getText(index));

    ui->textInput->activateWindow();
    ui->textInput->setFocus();

    switch (ui->scene->getRectType(index))
    {
    case TemplateCfg::RectType_Character:
        ui->textInput->setValidator(&characterValidator);
        ui->inputHint->setText("汉字");
        break;
    case TemplateCfg::RectType_Pinyin:
        ui->textInput->setValidator(&pinyinValidator);
        ui->inputHint->setText("拼音");
        break;
    case TemplateCfg::RectType_Letter:
        ui->textInput->setValidator(&letterValidator);
        ui->inputHint->setText("英文");
        break;
    case TemplateCfg::RectType_LetterNumber:
        ui->textInput->setValidator(&letterNumberValidator);
        ui->inputHint->setText("字母+数字");
        break;
    case TemplateCfg::RectType_Unit:
        ui->textInput->setValidator(&unitValidator);
        ui->inputHint->setText("单位");
        break;

    default:
        break;
    }
}

void BoardDesigner::onTextEdited(const QString &text)
{
    if (ui->scene->currentSelectedRect() == -1)
    {
        return;
    }
    ui->scene->setText(ui->scene->currentSelectedRect(), text);
}

void BoardDesigner::onTemplateSelected(const QString &templateName)
{
    TemplateCfg *cfg = TemplateCfg::create(QString("../../assets/textures/template/") + templateName + ".cfg");
    ui->scene->setCfg(cfg);
}

void BoardDesigner::initValidators()
{
    characterValidator.setRegularExpression(QRegularExpression(".*"));

    letterValidator.setRegularExpression(QRegularExpression("[a-zA-Z ]*"));

    letterNumberValidator.setRegularExpression(QRegularExpression("[a-zA-Z0-9 ]*"));

    pinyinValidator.setRegularExpression(QRegularExpression("[a-zA-Z ]*"));

    unitValidator.setRegularExpression(QRegularExpression("[a-zA-Z0-9 -]*"));
}