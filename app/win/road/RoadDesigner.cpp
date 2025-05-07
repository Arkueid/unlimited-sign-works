#include "RoadDesigner.hpp"

#include <win/road/PoleAddPanel.hpp>
#include <win/road/DateSetPanel.hpp>

#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QDateTime>

#include <climits>

RoadDesigner::RoadDesigner(QWidget *parent) : QWidget(parent), ui(new Ui::RoadDesigner), minDateTime(LLONG_MAX), maxDateTime(LLONG_MIN)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 6);

    connect(ui->addBtn, &QPushButton::clicked, this, &RoadDesigner::onAddPoleRequested);

    connect(ui->removeBtn, &QPushButton::clicked, this, &RoadDesigner::onRemovePoleRequested);

    connect(ui->nodeTree, &QTreeWidget::itemChanged, this, &RoadDesigner::onNodeStateChanged);

    connect(ui->saveBtn, &QPushButton::clicked, this, &RoadDesigner::onSaveBtnClicked);

    connect(ui->timeBtn, &QPushButton::clicked, this, &RoadDesigner::onSetTimeRequested);
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

void RoadDesigner::onPoleSubmitted(Pole *pole, float peg, const QString& slot, bool isLeft)
{
    PoleDrawable *d = ui->roadScene->addPole(pole, peg, isLeft);

    poleRoadData.append({NOT_COMPLETE, slot + (isLeft ? "（左）-" : "（右）-") + pole->getName().c_str(), peg, isLeft});

    QTreeWidgetItem *item = new QTreeWidgetItem(ui->nodeTree);
    item->setCheckState(0, Qt::Checked);
    item->setText(0, poleRoadData.back().slot);

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

    if (item == nullptr)
    {
        return;
    }

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

void RoadDesigner::onSaveBtnClicked()
{
    QFileDialog dialog(this);
	dialog.setNameFilter("Road JSON (*.json)");
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setFileMode(QFileDialog::AnyFile);
	QDir dir = QDir::current();
	dir.cdUp();
	dir.cdUp();
	// ../../assets/roads
	QString dirPath = dir.absoluteFilePath("assets/roads");

    dialog.setDirectory(dirPath);
	dialog.selectFile(ui->roadName->text() + ".json");
	if (dialog.exec())
	{
		QString filePath = dialog.selectedFiles().first();
		// qDebug() << "Saving to: " << filePath;
		toJsonFile(filePath);
	}
}

const QJsonObject RoadDesigner::toJson()
{
    QJsonObject root;
    const QVector<PoleDrawable*> drawables = ui->roadScene->getDrawables();
    // TODO
    return root;
}

void RoadDesigner::toJsonFile(const QString& fileName)
{
    const QJsonObject json = toJson();

    QFile file(fileName);
    if (file.open(QFile::WriteOnly))
    {
        QJsonDocument doc;
        doc.setObject(json);
        file.write(doc.toJson());
        file.close();
    }
}

void RoadDesigner::onSetTimeRequested()
{
    DateSetPanel* panel = new DateSetPanel(&poleRoadData);
    panel->show();
}