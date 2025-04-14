#pragma once

#include <ui_AddPanel.h>
#include <win/scene/PoleScene.hpp>
#include <QDoubleValidator>

class AddPanel: public QWidget {

    Ui::AddPanel* ui;
    PoleScene* scene;
    
    QDoubleValidator* doubleValidator;

    void initializeLineEdits();

    bool isParamsValid(const QVector<QLineEdit*>& numbers, const QVector<QLineEdit*>& texts);
    void addCube();
    void addCylinder();
    void addStiffener();
    void addRoundedCube();
    void addRoundedRegTriPrism();
public slots:
    void addPart();
public:
    AddPanel(PoleScene* scene, QWidget *parent = nullptr);
    ~AddPanel() override;
};