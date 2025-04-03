#include <win/texture/TextureBase.hpp>
#include <utils/TextureManager.hpp>
#include <QInputDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>

TextureBase::TextureBase(QWidget *parent): QWidget(parent), ui(new Ui::TextureBase)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 20);

    const QMap<int, QString> &texturesPaths = TextureManager::getTexturePaths();
    for (auto it = texturesPaths.begin(); it != texturesPaths.end(); ++it)
    {
        QListWidgetItem* textureItem = new QListWidgetItem(ui->textureList);
        textureItem->setText(it.value());
        textureItem->setData(Qt::UserRole, QVariant("../../assets/textures/" + it.value()));
        ui->textureList->addItem(textureItem);
    }

    connect(ui->textureList, &QListWidget::itemClicked, this, &TextureBase::onTextureSelected);

    connect(ui->addBtn, &QPushButton::clicked, this, &TextureBase::onAddBtnClicked);

    connect(ui->removeBtn, &QPushButton::clicked, this, &TextureBase::onRemoveBtnClicked);
}

TextureBase::~TextureBase()
{
    delete ui;
}

void TextureBase::onTextureSelected(QListWidgetItem* item)
{
    QString texturePath = item->data(Qt::UserRole).toString();
    QImage textureImage(texturePath);
    int w = ui->frame->width() * 0.75f;
    int h = ui->frame->height() * 0.75f;
    textureImage = textureImage.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->textureLabel->setPixmap(QPixmap::fromImage(textureImage));
}

void TextureBase::onAddBtnClicked()
{

    QString externalPath = QFileDialog::getOpenFileName(this, "选择纹理", "", "*.png *.jpg *.jpeg *.bmp");

    if (externalPath.isEmpty())
    {
        return;
    }

    QString defaultValue = QFileInfo(externalPath).fileName();
    QInputDialog dialog(this);
    dialog.setInputMode(QInputDialog::TextInput);
    dialog.setLabelText("已选择纹理：" + externalPath + "\n请输入纹理名称");
    dialog.setTextValue(defaultValue);
    dialog.setWindowTitle("添加纹理");
    dialog.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
    dialog.setCancelButtonText("取消");
    dialog.setOkButtonText("确定");
    dialog.adjustSize();

    if (dialog.exec() != QDialog::Accepted) 
    {
        return;
    }
    QString textureName = dialog.textValue();
    QString internalPath = "../../assets/textures/" + textureName;
    QFileInfo textureInfo(internalPath);
    if (textureInfo.exists())
    {
        QMessageBox::warning(this, "添加纹理", "纹理名称已存在");
        return;
    }

    // qDebug() << "externalPath:" << externalPath;
    // qDebug() << "textureName:" << textureName;

    int id = TextureManager::registerTexture(externalPath, textureName);
    // qDebug() << "id:" << id;

    QListWidgetItem* textureItem = new QListWidgetItem(ui->textureList);
    textureItem->setText(textureName);
    textureItem->setData(Qt::UserRole, QVariant(internalPath));
    ui->textureList->addItem(textureItem);
}

void TextureBase::onRemoveBtnClicked()
{
    QListWidgetItem* item = ui->textureList->currentItem();
    if (item == nullptr)
    {
        return;
    }

    QString textureName = item->text();
    if (TextureManager::unregisterTexture(textureName))
    {
        ui->textureList->takeItem(ui->textureList->row(item));
        delete item;
        ui->textureLabel->clear();
    }
}