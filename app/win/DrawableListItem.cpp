#include <win/DrawableListItem.hpp>

DrawableListItem::DrawableListItem(QWidget* parent): QWidget(parent), ui(new Ui::DrawableListItem)
{
	ui->setupUi(this);
}

DrawableListItem::~DrawableListItem()
{
	delete ui;
}
