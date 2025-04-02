#include <win/texture/TextureBase.hpp>
#include <utils/TextureManager.hpp>

TextureBase::TextureBase(QWidget *parent): QWidget(parent), ui(new Ui::TextureBase)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 20);

    const QMap<int, QString> &texturesPaths = TextureManager::getTexturePaths();
    for (auto it = texturesPaths.begin(); it != texturesPaths.end(); ++it)
    {
        ui->textureList->addItem(it.value());
    }

    connect(ui->textureList, &QListWidget::itemClicked, this, &TextureBase::onTextureSelected);
}

TextureBase::~TextureBase()
{
    delete ui;
}

void TextureBase::onTextureSelected(QListWidgetItem* item)
{
    QString texturePath = item->text();
    QImage textureImage(texturePath);
    int w = ui->frame->width() * 0.75f;
    int h = ui->frame->height() * 0.75f;
    textureImage = textureImage.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->textureLabel->setPixmap(QPixmap::fromImage(textureImage));
}