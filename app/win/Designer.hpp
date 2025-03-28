#pragma once

#include <ui_Designer.h>

class Designer: public QWidget
{
	Ui::DesignerWin* ui;
public:
	explicit Designer(QWidget* parent = nullptr);
	~Designer() override;

};

