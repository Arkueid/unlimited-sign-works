#include <win/scene/BoardScene.hpp>

#include <QPainter>

BoardScene::BoardScene(QWidget *parent) : QWidget(parent), cfg(nullptr), selectedRectIndex(-1), texts()
{
}

BoardScene::~BoardScene()
{
    if (cfg != nullptr)
    {
        delete cfg;
        cfg = nullptr;
    }
}

void BoardScene::paintEvent(QPaintEvent *event)
{
    if (originImage.isNull())
    {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);

    painter.fillRect(rect(), Qt::black);

    QPixmap boardImage = originImage.scaled(width(), height(), Qt::KeepAspectRatio);

    startX = (width() - boardImage.width()) / 2;
    startY = (height() - boardImage.height()) / 2;
    painter.drawPixmap(startX, startY, boardImage);

    if (cfg != nullptr)
    {
        scaleX = boardImage.width() / cfg->getWidth();
        scaleY = boardImage.height() / cfg->getHeight();

        const QVector<TemplateCfg::Rect *> &rects = cfg->getRects();
        for (int i = 0; i < rects.size(); i++)
        {
            TemplateCfg::Rect *rect = rects[i];

            const QString &text = texts[i];
            if (!text.isEmpty())
            {
                QRect fontRect(startX + rect->x * scaleX - 4, startY + rect->y * scaleY - 4, rect->w * scaleX + 8, rect->h * scaleY + 8);
                painter.fillRect(fontRect, getColor(rect->background));

                painter.setPen(getColor(rect->foreground));
                QFont font = painter.font();
                font.setPointSize(rect->h * scaleY * 0.5);
                painter.setFont(font);
                painter.drawText(fontRect, Qt::AlignCenter, text);
            }
        }

        if (selectedRectIndex != -1)
        {
            TemplateCfg::Rect *rect = rects[selectedRectIndex];
            QPen pen;
            if (rect->background == TemplateCfg::ColorType_R)
            {
                pen.setColor(Qt::yellow);
            }
            else
            {
                pen.setColor(Qt::red);
            }
            pen.setWidth(3);
            painter.setPen(pen);
            painter.drawRect(QRect(startX + rect->x * scaleX, startY + rect->y * scaleY, rect->w * scaleX, rect->h * scaleY));
        }
    }
}

void BoardScene::mousePressEvent(QMouseEvent *event)
{
    if (cfg == nullptr)
    {
        return;
    }

    const QVector<TemplateCfg::Rect *> &rects = cfg->getRects();
    selectedRectIndex = -1;
    int clickX = event->x();
    int clickY = event->y();
    for (int i = 0; i < rects.size(); i++)
    {
        TemplateCfg::Rect *rect = rects[i];
        int top = startY + rect->y * scaleY;
        int bottom = top + rect->h * scaleY;
        int left = startX + rect->x * scaleX;
        int right = left + rect->w * scaleX;
        if (clickX >= left && clickX <= right && clickY >= top && clickY <= bottom)
        {
            selectedRectIndex = i;
            break;
        }
    }

    update();

    emit selectionChanged(selectedRectIndex);
}

QColor BoardScene::getColor(TemplateCfg::ColorType type)
{
    switch (type)
    {
    case TemplateCfg::ColorType_W:
        return QColor(255, 255, 255);
    case TemplateCfg::ColorType_B:
        return QColor(0, 0, 0);
    case TemplateCfg::ColorType_G:
        return QColor(0, 135, 56);
    case TemplateCfg::ColorType_R:
        return QColor(255, 0, 0);
    case TemplateCfg::ColorType_Y:
        return QColor(255, 255, 0);
    case TemplateCfg::ColorType_Z:
        return QColor(128, 64, 0);
    case TemplateCfg::ColorType_O:
        return QColor(254, 153, 1);
    default:
        qDebug() << "unknown color type";
        return QColor(0, 0, 0);
    };
}

void BoardScene::setCfg(TemplateCfg *cfg)
{
    if (this->cfg != nullptr)
    {
        delete this->cfg;
        this->cfg = nullptr;
    }

    this->cfg = cfg;
    const QVector<TemplateCfg::Rect *> &rects = cfg->getRects();
    texts = QVector<QString>(rects.size(), "");

    originImage = QPixmap(QString("../../assets/textures/template/").append(cfg->getName()).append(".bmp"));

    selectedRectIndex = -1;

    if (isVisible())
    {
        update();
    }
}

void BoardScene::setTexutre(const QString &texturePath)
{
    if (this->cfg != nullptr)
    {
        delete this->cfg;
        this->cfg = nullptr;
    }

    texts.clear();

    originImage = QPixmap(texturePath);

    selectedRectIndex = -1;
    if (isVisible())
    {
        update();
    }
}

int BoardScene::currentSelectedRect() const
{
    return selectedRectIndex;
}
void BoardScene::setText(int index, const QString &text)
{
    texts[index] = text;
    update();
}

const QString &BoardScene::getText(int index) const
{
    return texts[index];
}

void BoardScene::exportImage(const QString &path)
{
    // 设置图片格式
    QImage image(originImage.width() * 1.1, originImage.height() * 1.1, QImage::Format_ARGB32);
    QPainter painter(&image);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);
    QPixmap scaledSrc = originImage.scaled(image.width(), image.height(), Qt::KeepAspectRatio);
    painter.drawPixmap(0, 0, scaledSrc); // 绘制底图
    if (cfg != nullptr)
    {
        float scaleX = image.width() / cfg->getWidth();
        float scaleY = image.height() / cfg->getHeight();
        const QVector<TemplateCfg::Rect *> &rects = cfg->getRects();
        for (int i = 0; i < texts.size(); i++)
        {                                       // 逐一绘制文字背景和文本
            TemplateCfg::Rect *rect = rects[i]; // 获取文字对应的矩形区域
            const QString &text = texts[i];     // 获取文字
            if (!text.isEmpty())
            {
                QRect fontRect(rect->x * scaleX - 2, rect->y * scaleY - 2,
                               rect->w * scaleX + 4, rect->h * scaleY + 4); // 校正矩形区域长宽误差
                painter.fillRect(fontRect, getColor(rect->background));     // 填充背景色
                painter.setPen(getColor(rect->foreground));                 // 设置文本颜色
                QFont font = painter.font();
                font.setPointSize(rect->h * scaleY * 0.5); // 设置字体大小
                painter.setFont(font);
                painter.drawText(fontRect, Qt::AlignCenter, text); // 绘制文字
            }
        }
    }
    image.save(path); // 保存图片为.png文件
}

TemplateCfg::RectType BoardScene::getRectType(int index) const
{
    return (TemplateCfg::RectType)cfg->getRects()[index]->type;
}