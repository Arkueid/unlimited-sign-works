#pragma once

#include <ui_TextureBase.h>


class TextureBase : public QWidget {
    Q_OBJECT

    Ui::TextureBase* ui;

	void loadTextureList();
public:
    TextureBase(QWidget* parent = nullptr);
    ~TextureBase() override;
public slots:
    void onTextureSelected(QListWidgetItem* item);
    void onAddBtnClicked();
    void onRemoveBtnClicked();
    void onSearchInputTextEdited(const QString& text);
};