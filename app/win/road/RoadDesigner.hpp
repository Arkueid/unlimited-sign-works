#pragma once

#include "ui_RoadDesigner.h"


class RoadDesigner : public QWidget
{

    Q_OBJECT
    Ui::RoadDesigner *ui;

private slots:
    void onAddPoleRequested();

    void onPoleSubmitted(Pole* pole);

    void onNodeStateChanged(QTreeWidgetItem* item, int column);

    void onRemovePoleRequested();

public:
    RoadDesigner(QWidget *parent = nullptr);
    ~RoadDesigner() override;

    QLineEdit* getRoadNameLineEdit();
};