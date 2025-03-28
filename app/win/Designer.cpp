#include <win/Designer.hpp>

Designer::Designer(QWidget* parent): QWidget(parent), ui(new Ui::DesignerWin)
{
	ui->setupUi(this);
	ui->splitter->setStretchFactor(0, 8);
	ui->splitter->setStretchFactor(1, 1);
}

Designer::~Designer()
{
	delete ui;
}
