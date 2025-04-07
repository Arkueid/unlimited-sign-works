#pragma once

#include <QVector>
#include <QString>

class TemplateCfg
{
    TemplateCfg() = default;

public:
    enum ColorType
    {
        ColorType_W, // 白色
        ColorType_G, // 绿色
        ColorType_B, // 黑色
        ColorType_Y, // 黄色
        ColorType_R, // 红色
        ColorType_Z, // 棕色
    };

    struct Rect
    {
        int type;
        ColorType foreground; // 文字颜色
        ColorType background; // 背景填充颜色
        float x;              // 左上角x
        float y;              // 左上角y
        float w;
        float h;
    };

    enum RectType
    {
        RectType_Character = 1,    // 汉字
        RectType_Pinyin = 2,       // 拼音
        RectType_Letter = 3,       // 字母
        RectType_LetterNumber = 4, // 字母数字
        RectType_Unit = 5,         // 单位
    };

    ~TemplateCfg();

    static TemplateCfg *create(const QString &cfgPath);

    const QVector<Rect *> &getRects() const;

    const QString &getName() const;

    float getWidth() const;

    float getHeight() const;

private:
    QString name;
    float width;
    float height;
    QVector<Rect *> rects;
};