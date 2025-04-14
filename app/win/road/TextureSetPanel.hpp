#pragma once

#include "ui_TextureSetPanel.h"

#include <part/Part.hpp>


class TextureSetPanel : public QWidget
{

    Q_OBJECT

    void initTemplateSelector();
    void initValidators();
    void initTextureSelector();

signals:
    void success(const QString& textureName);

public slots:
    void onConfirmBtnClicked();
    void onSelectionChanged(int index);
    void onTextEdited(const QString &text);
    void onTemplateSelected(const QString &templateName);
    void onTypeChanged(int index);

public:
    TextureSetPanel(Part* part, int drawableIndex, QWidget *parent = nullptr);
    ~TextureSetPanel() override;

private:
    Ui::TextureSetPanel *ui;

    QRegularExpressionValidator characterValidator;
    QRegularExpressionValidator pinyinValidator;
    QRegularExpressionValidator letterValidator;
    QRegularExpressionValidator letterNumberValidator;
    QRegularExpressionValidator unitValidator;

    Part* part;
    int drawableIndex;
};