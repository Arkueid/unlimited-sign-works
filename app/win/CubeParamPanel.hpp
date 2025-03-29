#pragma once

#include <ui_CubeParamPanel.h>
#include <base/Mesh.hpp>
#include <part/Part.hpp>
#include <win/scene/PoleScene.hpp>


class CubeParamPanel: public QWidget
{
    Q_OBJECT

    Ui::CubeParamPanel* ui;

    PoleScene* scene;

    bool isParamValid();
public:
    CubeParamPanel(PoleScene* scene,
        QWidget* parent = nullptr);

    ~CubeParamPanel();
public slots:
    void onSubmit();
};