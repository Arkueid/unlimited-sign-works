#pragma once

#include "ui_RoadDesigner.h"


class RoadDesigner : public QWidget
{

    Q_OBJECT
    Ui::RoadDesigner *ui;

private slots:
    void onAddPoleRequested();

public:
    RoadDesigner(QWidget *parent = nullptr);
    ~RoadDesigner() override;

    QLineEdit* getRoadNameLineEdit();
};