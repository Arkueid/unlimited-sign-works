#pragma once

#include <ui_PoleDesigner.h>
#include <QTimer>

class PoleDesigner: public QWidget
{
	Ui::PoleDesigner* ui;

	QTimer timer;
public:
	explicit PoleDesigner(QWidget* parent = nullptr);
	~PoleDesigner() override;

};

