#pragma once

#include "ui_RoadDesigner.h"
#include <QJsonObject>

#include "Common.hpp"

class RoadDesigner : public QWidget
{

    Q_OBJECT
    Ui::RoadDesigner *ui;

    QVector<PoleRoadData> poleRoadData;

    long long minDateTime;
    long long maxDateTime;

private slots:
    void onAddPoleRequested();

    void onPoleSubmitted(Pole* pole, float peg, const QString& slot, bool isLeft);

    void onNodeStateChanged(QTreeWidgetItem* item, int column);

    void onRemovePoleRequested();

    void onSaveBtnClicked();

    void onSetTimeRequested();

public:
    RoadDesigner(QWidget *parent = nullptr);
    ~RoadDesigner() override;

    QLineEdit* getRoadNameLineEdit();
private:
    const QJsonObject toJson();
    void toJsonFile(const QString& fileName);
};