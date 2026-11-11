#include "RoadDesigner.hpp"

#include <win/road/PoleAddPanel.hpp>
#include <win/road/DateSetPanel.hpp>
#include <utils/PoleJsonSerializer.hpp>

#include <QMessageBox>
#include <QFileDialog>
#include <QJsonDocument>
#include <QDateTime>
#include <QJsonArray>

#include <climits>
#include <algorithm>

#include <QJsonObject>
#include <QJsonDocument>
#include <mysql/MySQLManager.hpp>

RoadDesigner::RoadDesigner(QWidget *parent, const QString &fileName, int roadId) : QWidget(parent), ui(new Ui::RoadDesigner), minDateTime(LLONG_MAX), maxDateTime(LLONG_MIN),
                                                                       roadFileName(fileName), roadId(roadId)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 6);

    ui->timeSlider->setRange(0, 100);

    connect(ui->timeSlider, &QSlider::valueChanged, this, &RoadDesigner::onTimeSliderValueChanged);

    connect(ui->addBtn, &QPushButton::clicked, this, &RoadDesigner::onAddPoleRequested);

    connect(ui->removeBtn, &QPushButton::clicked, this, &RoadDesigner::onRemovePoleRequested);

    connect(ui->nodeTree, &QTreeWidget::itemChanged, this, &RoadDesigner::onNodeStateChanged);

    connect(ui->saveBtn, &QPushButton::clicked, this, &RoadDesigner::onSaveBtnClicked);

    connect(ui->timeBtn, &QPushButton::clicked, this, &RoadDesigner::onSetTimeRequested);

    connect(ui->roadScene, &RoadScene::initialized, this, &RoadDesigner::onSceneInitialized);

    connect(ui->editBtn, &QPushButton::clicked, this, &RoadDesigner::onEditBtnClicked);
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

void RoadDesigner::onPoleSubmitted(Pole *pole, float peg, const QString &slot, bool isLeft)
{
    addPole(pole, peg, isLeft, slot + (isLeft ? "（左）-" : "（右）-") + pole->getName().c_str(), NOT_COMPLETE);
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

            poleRoadData.removeAt(ui->nodeTree->indexOfTopLevelItem(item));

            delete item;
        }
    }
}

void RoadDesigner::onSaveBtnClicked()
{
    QString roadName = ui->roadName->text();
    if (roadName.isEmpty())
    {
        QMessageBox::warning(this, "错误", "请输入道路名称");
        return;
    }

    QSqlDatabase db = MySQLManager::getDatabase();
    QSqlQuery query(db);
    query.prepare("select id from road where name = ?");
    query.addBindValue(roadName);
    if (!query.exec())
    {
        qDebug() << query.lastError();
        return;
    }

    if (query.next())
    {
        const int id = query.value(0).toInt();
        if (QMessageBox::Ok != QMessageBox::question(this, "警告", "道路已存在，是否覆盖？"))
        {
            return;
        }

        QJsonObject root;
        toJson(root);
        QJsonDocument doc(root);
        query.prepare("update road set json_data = ? where id = ?");
        query.addBindValue(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
        query.addBindValue(id);
        if (!query.exec())
        {
            qDebug() << query.lastError();
        }
        return;
    }
    else 
    {
        QJsonObject root;
        toJson(root);
        QJsonDocument doc(root);
        query.prepare("insert into road(name, json_data) values(?, ?)");
        query.addBindValue(roadName);
        query.addBindValue(QString::fromUtf8(doc.toJson(QJsonDocument::Compact)));
        if (!query.exec())
        {
            qDebug() << query.lastError();
        }
        else 
        {
            QMessageBox::information(this, "提示", "道路保存成功");
        }
    }
}

void RoadDesigner::toJson(QJsonObject &root)
{
    root["name"] = ui->roadName->text();
    QJsonArray poles;

    const QVector<PoleDrawable *> drawables = ui->roadScene->getDrawables();
    const int drawableCount = drawables.size();
    for (int i = 0; i < drawableCount; i++)
    {
        Pole *pole = drawables[i]->getPole();
        QJsonObject poleJson;
        PoleJsonSerializer::toJson(pole, poleJson);
        poleJson["peg"] = poleRoadData[i].peg;
        poleJson["slot"] = poleRoadData[i].slot;
        poleJson["cdt"] = poleRoadData[i].completeDateTime;
        poleJson["isLeft"] = poleRoadData[i].isLeft;

        poles.append(poleJson);
    }

    root["poles"] = poles;
}

void RoadDesigner::toJsonFile(const QString &fileName)
{
    QJsonObject root;
    toJson(root);

    QFile file(fileName);
    if (file.open(QFile::WriteOnly))
    {
        QJsonDocument doc;
        doc.setObject(root);
        file.write(doc.toJson());
        file.close();
    }
}

void RoadDesigner::onSetTimeRequested()
{
    DateSetPanel *panel = new DateSetPanel(&poleRoadData);
    panel->show();

    connect(panel, &DateSetPanel::committed,
            [&](long long dt)
            {
                maxDateTime = std::max(dt, maxDateTime);
                minDateTime = std::min(dt, minDateTime);
            });
}

void RoadDesigner::onTimeSliderValueChanged(int value)
{
    // 前后额外增加一天
    const long long offset = 60 * 60 * 24;
    long long dt = (maxDateTime - minDateTime + 2 * offset) * value / 100 + minDateTime - offset;
    ui->dateTimeEdit->setDateTime(QDateTime::fromSecsSinceEpoch(dt));

    const QVector<PoleDrawable *> &drawables = ui->roadScene->getDrawables();
    const int size = drawables.size();
    ui->nodeTree->blockSignals(true);
    for (int i = 0; i < size; i++)
    {
        if (poleRoadData[i].completeDateTime == NOT_COMPLETE)
        {
            continue;
        }

        if (poleRoadData[i].completeDateTime > dt)
        {
            drawables[i]->setVisible(false);
            ui->nodeTree->topLevelItem(i)->setCheckState(0, Qt::Unchecked);
        }
        else
        {
            drawables[i]->setVisible(true);
            ui->nodeTree->topLevelItem(i)->setCheckState(0, Qt::Checked);
        }
    }
    ui->nodeTree->blockSignals(false);

    ui->roadScene->update();
}

void RoadDesigner::fromJsonFile(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();
        fromJson(doc.object());
    }
}

void RoadDesigner::fromJson(const QJsonObject &root)
{
    const QJsonArray poles = root["poles"].toArray();

    const int size = poles.size();
    for (int i = 0; i < size; i++)
    {
        QJsonObject poleJson = poles[i].toObject();
        Pole *pole = PoleJsonSerializer::fromJson(poleJson);
        float peg = poleJson["peg"].toDouble();
        bool isLeft = poleJson["isLeft"].toBool();
        long long cdt = poleJson["cdt"].toInteger();
        const QString &slot = poleJson["slot"].toString();
        addPole(pole, peg, isLeft, slot, cdt);
    }

    ui->roadName->setText(root["name"].toString());
}

void RoadDesigner::addPole(Pole *pole, float peg, bool isLeft, const QString &slot, long long cdt)
{
    PoleDrawable *d = ui->roadScene->addPole(pole, peg, isLeft);

    poleRoadData.append({cdt, slot, peg, isLeft});

    QTreeWidgetItem *item = new QTreeWidgetItem(ui->nodeTree);
    item->setCheckState(0, Qt::Checked);
    item->setText(0, poleRoadData.back().slot);

    item->setData(0, Qt::UserRole, QVariant::fromValue(d));

    if (cdt > 0)
    {
        maxDateTime = std::max(cdt, maxDateTime);
        minDateTime = std::min(cdt, minDateTime);
    }
}

void RoadDesigner::onSceneInitialized()
{
    if (!roadFileName.isEmpty())
    {
        fromJsonFile(roadFileName);
    }
    else if (roadId > 0)
    {
        QSqlDatabase db = MySQLManager::getDatabase();
        QSqlQuery query(db);
        query.prepare("select json_data from road where id = ?");
        query.addBindValue(roadId);
        if (query.exec())
        {
            query.next();
            QJsonDocument doc = QJsonDocument::fromJson(query.value(0).toByteArray());
            fromJson(doc.object());
        }
        else 
        {
            qDebug() << query.lastError();
        }
    }
}

void RoadDesigner::onEditBtnClicked()
{
    PoleAddPanel* panel = new PoleAddPanel(this, true);
    QTreeWidgetItem* item = ui->nodeTree->currentItem();
    if (item == nullptr)
    {
        return;
    }
    // 获取选中的标识牌节点索引
    const int index = ui->nodeTree->indexOfTopLevelItem(item);
    // 向编辑面板传递要修改的标识牌数据
    panel->setPole(ui->roadScene->getPoleDrawable(index)->getPole(),
        poleRoadData[index].slot.split("（").first(), poleRoadData[index].isLeft);
    // 将面板的提交操作绑定到标识牌修改的业务逻辑上
    connect(panel, &PoleAddPanel::poleSubmitted, this, &RoadDesigner::onPoleModified);
    panel->show();  // 显示标识牌编辑面板
}

void RoadDesigner::onPoleModified(Pole *pole, float peg, const QString &slot, bool isLeft)
{
    QTreeWidgetItem *item = ui->nodeTree->currentItem();
    PoleDrawable *poleDrawable = item->data(0, Qt::UserRole).value<PoleDrawable *>();
    QMatrix4x4 model = ui->roadScene->placePole(pole, peg, isLeft);
    poleDrawable->setModelMatrix(model);
    Pole* currentPole = poleDrawable->getPole();

    ui->roadScene->changePole(poleDrawable, pole);
    
    const int index = ui->nodeTree->indexOfTopLevelItem(item);
    poleRoadData[index].peg = peg;
    poleRoadData[index].slot = slot + (isLeft ? "（左）-" : "（右）-") + pole->getName().c_str();
    poleRoadData[index].isLeft = isLeft;

    item->setText(0, poleRoadData[index].slot);

    ui->roadScene->update();
}
