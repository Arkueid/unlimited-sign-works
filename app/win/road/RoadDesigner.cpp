#include "RoadDesigner.hpp"

#include <win/road/PoleAddPanel.hpp>
#include <QMessageBox>

RoadDesigner::RoadDesigner(QWidget *parent) : QWidget(parent), ui(new Ui::RoadDesigner)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 6);

    connect(ui->addBtn, &QPushButton::clicked, this, &RoadDesigner::onAddPoleRequested);

    connect(ui->removeBtn, &QPushButton::clicked, this, &RoadDesigner::onRemovePoleRequested);

    connect(ui->nodeTree, &QTreeWidget::itemChanged, this, &RoadDesigner::onNodeStateChanged);
}

RoadDesigner::~RoadDesigner()
{
    delete ui;
}

QLineEdit *RoadDesigner::getRoadNameLineEdit()
{
    return ui->roadName;
}

void RoadDesigner::onAddPoleRequested()
{
    PoleAddPanel *panel = new PoleAddPanel(this);
    panel->show();

    connect(panel, &PoleAddPanel::poleSubmitted, this, &RoadDesigner::onPoleSubmitted);
}

void RoadDesigner::onPoleSubmitted(Pole *pole)
{
    PoleDrawable *d = ui->roadScene->addPole(pole);

    QTreeWidgetItem *item = new QTreeWidgetItem(ui->nodeTree);
    item->setCheckState(0, Qt::Checked);
    item->setText(0, pole->getName().c_str());

    item->setData(0, Qt::UserRole, QVariant::fromValue(d));
}

void RoadDesigner::onNodeStateChanged(QTreeWidgetItem *item, int column)
{
    if (item->parent() == nullptr)
    {
        int index = ui->nodeTree->indexOfTopLevelItem(item);
        ui->roadScene->getPoleDrawable(index)->setVisible(item->checkState(column) == Qt::Checked);
        ui->roadScene->update();
    }
}

void RoadDesigner::onRemovePoleRequested()
{
    QTreeWidgetItem *item = ui->nodeTree->currentItem();

    if (item->parent() == nullptr)
    {
        if (QMessageBox::question(this, "删除节点", "是否确定删除节点：" + item->text(0)) == QMessageBox::Yes)
        {
            PoleDrawable *poleDrawable = item->data(0, Qt::UserRole).value<PoleDrawable *>();

            ui->roadScene->removePole(poleDrawable);

            delete item;
        }
    }
}
