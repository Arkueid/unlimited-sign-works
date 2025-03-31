#pragma once

#include <ui_CubePanel.h>
#include <base/Mesh.hpp>
#include <part/Part.hpp>
#include <win/scene/PoleScene.hpp>


class CubePanel: public QWidget
{
    Q_OBJECT

    Ui::CubePanel* ui;

    PoleScene* scene;

    bool isParamValid();
public:
    CubePanel(PoleScene* scene,
        QWidget* parent = nullptr);

    ~CubePanel();
public slots:
    void onSubmit();
};