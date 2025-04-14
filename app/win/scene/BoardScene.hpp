#pragma once

#include <QWidget>
#include <QVector>
#include <QMouseEvent>

#include <template/TemplateCfg.hpp>


class BoardScene : public QWidget {

    Q_OBJECT

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    QColor getColor(TemplateCfg::ColorType type);

signals:
    void selectionChanged(int index);
public:
    BoardScene(QWidget *parent = nullptr);
    ~BoardScene();

    void setCfg(TemplateCfg *cfg);

    void setTexutre(const QString& texturePath);

    int currentSelectedRect() const;

    void setText(int index, const QString &text);

    const QString& getText(int index) const;

    TemplateCfg::RectType getRectType(int index) const;

private:
    TemplateCfg *cfg;

    QVector<QString> texts;

    QPixmap originImage;

    int selectedRectIndex;

    float scaleX;
    float scaleY;
    int startX;  // 面板左上角绘制坐标x
    int startY;  // 面板左上角绘制坐标y

public slots:
    void exportImage(const QString &path);
};