#include "RoadDesigner.hpp"

#include <win/road/PoleAddPanel.hpp>

RoadDesigner::RoadDesigner(QWidget *parent): QWidget(parent), ui(new Ui::RoadDesigner)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 6);

    connect(ui->addBtn, &QPushButton::clicked, this, &RoadDesigner::onAddPoleRequested);
}

RoadDesigner::~RoadDesigner()
{
    delete ui;
}

QLineEdit * RoadDesigner::getRoadNameLineEdit()
{
    return ui->roadName;
}

void RoadDesigner::onAddPoleRequested()
{
    PoleAddPanel* panel = new PoleAddPanel(this);
    panel->show();
}
