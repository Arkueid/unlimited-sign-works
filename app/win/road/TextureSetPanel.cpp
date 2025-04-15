#include "TextureSetPanel.hpp"

#include <utils/TextureManager.hpp>

#include <QDir>
#include <QMessageBox>
#include <QUuid>


TextureSetPanel::TextureSetPanel(Part* part, int drawableIndex, QWidget *parent) : QWidget(parent), ui(new Ui::TextureSetPanel), part(part), drawableIndex(drawableIndex)
{
    ui->setupUi(this);

    initTemplateSelector();
    initTextureSelector();
    initValidators();

    connect(ui->scene, &BoardScene::selectionChanged, this, &TextureSetPanel::onSelectionChanged);

    connect(ui->textInput, &QLineEdit::textEdited, this, &TextureSetPanel::onTextEdited);

    connect(ui->confirmBtn, &QPushButton::clicked, this, &TextureSetPanel::onConfirmBtnClicked);

    connect(ui->typeSelector, &QComboBox::currentIndexChanged, this, &TextureSetPanel::onTypeChanged);

    ui->pages->setCurrentWidget(ui->pageGeneral);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::ApplicationModal);
}

TextureSetPanel::~TextureSetPanel()
{
    delete ui;
}


void TextureSetPanel::initTemplateSelector()
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

    connect(ui->templateSelector, &QComboBox::currentTextChanged, this, &TextureSetPanel::onTemplateSelected);
}

void TextureSetPanel::initValidators()
{
    characterValidator.setRegularExpression(QRegularExpression("."));

    letterValidator.setRegularExpression(QRegularExpression("[a-zA-Z ]*"));

    letterNumberValidator.setRegularExpression(QRegularExpression("[a-zA-Z0-9 ]*"));

    pinyinValidator.setRegularExpression(QRegularExpression("[a-zA-Z ]*"));

    unitValidator.setRegularExpression(QRegularExpression("[a-zA-Z0-9 -]*"));
}

void TextureSetPanel::initTextureSelector()
{
    const QMap<int, QString>& texturePaths = TextureManager::getTexturePaths();

    for (auto it = texturePaths.begin(); it != texturePaths.end(); ++it)
    {
        ui->textureSelector->addItem(it.value(), it.key());
    }

    ui->textureSelector->setCurrentIndex(-1);
    connect(ui->textureSelector, &QComboBox::currentIndexChanged,
				[=](int index)
				{
                    if (index == -1)
                    {
                        return;
                    }
                    const QString path = "../../assets/textures/" + texturePaths.value(ui->textureSelector->itemData(index).toInt());
                    ui->scene->setTexutre(path);
				});
}

void TextureSetPanel::onConfirmBtnClicked()
{
    QString textureName;

    if (ui->typeSelector->currentIndex() == 0)
    {
        if (ui->textureSelector->currentIndex() == -1)
        {
            QMessageBox::warning(this, "警告", "请选择纹理");
            return;
        }

        int texId = ui->textureSelector->currentData().toInt();
        textureName = TextureManager::getTexturePaths()[texId];
        part->setTextureRef(drawableIndex, texId);
    }
    else if (ui->typeSelector->currentIndex() == 1)
    {
        if (ui->templateSelector->currentIndex() == -1)
        {
            QMessageBox::warning(this, "警告", "请选择模板");
            return;
        }
        const QString& outputFile = QUuid::createUuid().toString(QUuid::WithoutBraces) + ".png";
        textureName = outputFile;
        const QString& externalPath = "../../assets/textures/generated/" + outputFile;
        ui->scene->exportImage(externalPath);
        int texId = TextureManager::registerTexture("../../assets/textures/generated/" + outputFile, outputFile, true);
        if (texId != -1)
        {
            part->setTextureRef(drawableIndex, texId);
        }
        else 
        {
            QMessageBox::warning(this, "警告", "纹理创建失败");
            return;
        }
    }
    emit success(textureName);
    close();
}

void TextureSetPanel::onSelectionChanged(int index)
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

void TextureSetPanel::onTextEdited(const QString &text)
{
    if (ui->scene->currentSelectedRect() == -1)
    {
        return;
    }
    ui->scene->setText(ui->scene->currentSelectedRect(), text);
}

void TextureSetPanel::onTemplateSelected(const QString &templateName)
{
    if (templateName == "")
    {
        return;
    }
    TemplateCfg *cfg = TemplateCfg::create(QString("../../assets/textures/template/") + templateName + ".cfg");
    ui->scene->setCfg(cfg);
}

void TextureSetPanel::onTypeChanged(int index)
{
    ui->pages->setCurrentIndex(index);
    
    ui->templateSelector->setCurrentIndex(-1);
    ui->textureSelector->setCurrentIndex(-1);

    ui->scene->setTexutre("");
}
