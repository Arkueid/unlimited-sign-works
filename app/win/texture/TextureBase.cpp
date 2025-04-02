#include <win/texture/TextureBase.hpp>


TextureBase::TextureBase(QWidget *parent): QWidget(parent), ui(new Ui::TextureBase)
{
    ui->setupUi(this);
}

TextureBase::~TextureBase()
{
    delete ui;
}
