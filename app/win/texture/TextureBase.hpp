#pragma once

#include <ui_TextureBase.h>


class TextureBase : public QWidget {
    Q_OBJECT

    Ui::TextureBase* ui;
public:
    TextureBase(QWidget* parent = nullptr);
    ~TextureBase() override;
public slots:
    void onTextureSelected(QListWidgetItem* item);
    void onAddBtnClicked();
    void onRemoveBtnClicked();
};