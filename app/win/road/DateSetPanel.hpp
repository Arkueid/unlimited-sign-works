#pragma once

#include "ui_DateSetPanel.h"

#include "Common.hpp"

class DateSetPanel : public QWidget
{
    Q_OBJECT

    Ui::DateSetPanel* ui;

    QVector<PoleRoadData>* poleRoadData;

signals:
    void committed(const long long);
    void reverted();

public:
    explicit DateSetPanel(QVector<PoleRoadData>* poleRoadData, QWidget* parent = nullptr);
    ~DateSetPanel();

private:
    void initData();

    void onCommitRequested();

    void onRevertRequested();

    void addToUList(PoleRoadData& data, int index);

    void addToFList(PoleRoadData& data, int index);
};