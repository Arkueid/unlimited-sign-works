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

    const QString& roadFileName;

private slots:
    void onAddPoleRequested();

    void onPoleSubmitted(Pole* pole, float peg, const QString& slot, bool isLeft);

    void onNodeStateChanged(QTreeWidgetItem* item, int column);

    void onRemovePoleRequested();

    void onSaveBtnClicked();

    void onSetTimeRequested();

    void onTimeSliderValueChanged(int value);

    void onSceneInitialized();

    void onEditBtnClicked();

    void onPoleModified(Pole* pole, float peg, const QString& slot, bool isLeft);

public:
    RoadDesigner(QWidget *parent = nullptr, const QString& fileName = "");
    ~RoadDesigner() override;

    QLineEdit* getRoadNameLineEdit();

    void fromJsonFile(const QString& fileName);
private:
    void toJson(QJsonObject& root);
    void toJsonFile(const QString& fileName);

    void fromJson(const QJsonObject& root);

    void addPole(Pole* pole, float peg, bool isLeft, const QString& slot, long long cdt);
};