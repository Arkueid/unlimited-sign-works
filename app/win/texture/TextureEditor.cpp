#include <win/texture/TextureEditor.hpp>


TextureEditor::TextureEditor(QWidget *parent): QWidget(parent), ui(new Ui::TextureEditor)
{
    ui->setupUi(this);
}

TextureEditor::~TextureEditor()
{
    delete ui;
}
