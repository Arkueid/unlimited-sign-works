#pragma once

#include <ui_TextureEditor.h>

class TextureEditor : public QWidget {
    Q_OBJECT
    Ui::TextureEditor* ui;
public: 
    TextureEditor(QWidget* parent = nullptr);
    ~TextureEditor() override;
};