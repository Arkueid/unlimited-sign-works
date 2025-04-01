#pragma once

#include <ui_DrawableListItem.h>


class DrawableListItem : public QWidget
{
	Ui::DrawableListItem* ui;
public:
	DrawableListItem(QWidget* parent = nullptr);
	~DrawableListItem() override;

	QCheckBox* getCheckBox()
	{
		return ui->visibleCheckBox;
	}

	QLabel* getName()
	{
		return ui->name;
	}
};