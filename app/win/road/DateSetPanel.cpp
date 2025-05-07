#include "DateSetPanel.hpp"

#include "DateTimeDialog.hpp"

#include <QDateTime>

DateSetPanel::DateSetPanel(QVector<PoleRoadData> *poleRoadData, QWidget *parent) : QWidget(parent), ui(new Ui::DateSetPanel), poleRoadData(poleRoadData)
{
    ui->setupUi(this);

    setWindowModality(Qt::WindowModality::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose);

    connect(ui->commitBtn, &QPushButton::clicked, this, &DateSetPanel::onCommitRequested);
    connect(ui->revertBtn, &QPushButton::clicked, this, &DateSetPanel::onRevertRequested);

    initData();
}

DateSetPanel::~DateSetPanel()
{
    delete ui;
}

void DateSetPanel::initData()
{
    int index = 0;
    for (auto& data: (*poleRoadData))
    {
        if (data.completeDateTime == NOT_COMPLETE)
        {
            addToUList(data, index);
        }
        else
        {
           addToFList(data, index);
        }
        index++;
    }
}

void DateSetPanel::onCommitRequested()
{
    const QListWidgetItem* item = ui->uList->currentItem();
    if (item == nullptr)
    {
        return;
    }

    DateTimeDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
    {
        return;
    }

    const long long ct = dialog.getDateTime().toSecsSinceEpoch();
    const int index = item->data(Qt::UserRole).toInt();
    PoleRoadData& data = (*poleRoadData)[index];
    data.completeDateTime = ct;
    ui->uList->takeItem(ui->uList->currentRow());

    addToFList(data, index);
    emit committed(ct);
}

void DateSetPanel::onRevertRequested()
{
    QListWidgetItem* item = ui->fList->currentItem();
    if (item == nullptr)
    {
        return;
    }

    const int index = item->data(Qt::UserRole).toInt();
    PoleRoadData& data = (*poleRoadData)[index];
    data.completeDateTime = NOT_COMPLETE;
    ui->fList->takeItem(ui->fList->currentRow());
    
    addToUList(data, index);
    emit reverted();
}

void DateSetPanel::addToUList(PoleRoadData &data, int index)
{
    QListWidgetItem* item = new QListWidgetItem(ui->uList);
    item->setText(data.slot);
    item->setData(Qt::ItemDataRole::UserRole, QVariant(index));
    ui->uList->addItem(item);
}

void DateSetPanel::addToFList(PoleRoadData &data, int index)
{
    QListWidgetItem* item = new QListWidgetItem(ui->fList);
    item->setText(data.slot);
    item->setData(Qt::ItemDataRole::UserRole, QVariant(index));
    ui->fList->addItem(item);
}
