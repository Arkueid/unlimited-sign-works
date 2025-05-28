#include <template/TemplateCfg.hpp>

#include <QFile>
#include <QDebug>

static TemplateCfg::ColorType Char2ColorType(char c)
{
    switch (c)
    {
    case 'B':
        return TemplateCfg::ColorType::ColorType_B;
    case 'G':
        return TemplateCfg::ColorType::ColorType_G;
    case 'R':
        return TemplateCfg::ColorType::ColorType_R;
    case 'W':
        return TemplateCfg::ColorType::ColorType_W;
    case 'Y':
        return TemplateCfg::ColorType::ColorType_Y;
    case 'Z':
        return TemplateCfg::ColorType::ColorType_Z;
    case 'O':
        return TemplateCfg::ColorType::ColorType_O;
    default:
        return TemplateCfg::ColorType::ColorType_B;
    }
}

TemplateCfg::~TemplateCfg()
{
    for (auto rect : rects)
    {
        delete rect;
    }
    rects.clear();
}

TemplateCfg *TemplateCfg::create(const QString &cfgPath)
{
    QFile file(cfgPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return nullptr;
    }

    TemplateCfg *cfg = new TemplateCfg();

    QByteArray data = file.readLine();
    QByteArrayList strs = data.split(' ');

    cfg->name = strs[0];

    strs = strs[1].split('*');
    cfg->width = strs[0].toFloat();
    cfg->height = strs[1].toFloat();

    int type;
    ColorType foreground, background;
    int num;
    float w, h;
    while (!file.atEnd())
    {
        data = file.readLine();
        strs = data.split(' ');
        type = strs[0].toInt();
        foreground = Char2ColorType(strs[1][0]);
        background = Char2ColorType(strs[2][0]);
        num = strs[3].toInt();
        strs = strs[4].split('*');
        w = strs[0].toFloat();
        h = strs[1].toFloat();
        for (int i = 0; i < num; i++)
        {
            TemplateCfg::Rect *rect = new TemplateCfg::Rect();
            rect->type = type;
            rect->foreground = foreground;
            rect->background = background;
            rect->h = h;
            rect->w = w;
            data = file.readLine();
            strs = data.split(' ');
            strs.removeIf([](const QByteArray &str)
                          { return str.isEmpty(); });
            rect->x = strs[0].toFloat();
            rect->y = cfg->height - strs[1].toFloat() - h;
            cfg->rects.push_back(rect);
        }
    }

    file.close();

    // for (auto rect : cfg->rects)
    // {
    //     qDebug() << "type: " << rect->type << " foreground: " << rect->foreground << " background: " << rect->background << " x: " << rect->x << " y: " << rect->y << " w: " << rect->w << " h: " << rect->h;
    // }
    return cfg;
}

const QVector<TemplateCfg::Rect *> &TemplateCfg::getRects() const
{
    return rects;
}

const QString &TemplateCfg::getName() const
{
    return name;
}

float TemplateCfg::getWidth() const
{
    return width;
}

float TemplateCfg::getHeight() const
{
    return height;
}
