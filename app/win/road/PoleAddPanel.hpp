#pragma once


#include "ui_PoleAddPanel.h"

#include <win/scene/PoleScene.hpp>

#include <QDoubleValidator>
#include <QMetaObject>
#include <QDialog>

struct BaseInfo
{
    Part* part;
    int drawableIndex;
};

class PoleAddPanel : public QDialog {

    Q_OBJECT

    void initNodeTree();

    void initTexturePanel();

    void initSizes();

    void bindPart(Part* part);

	void bindCube(Part* part);
	void bindCylinder(Part* part);
	void bindStiffener(Part* part);
	void bindRoundedCube(Part* part);
	void bindRoundedRegTriPrism(Part* part);

	void hidePanels();

	void initializeLineEdits();

	void bindTransforms(Part* part);

    void refreshMesh(Part* part, Mesh* mesh);

    void refreshMesh(BaseInfo& base, Mesh* mesh);

    bool isPoleSet();

    bool isAllTexSet();

    bool isBaseSet();

private slots:
    void onNodeClicked(QTreeWidgetItem* item, int column);

    void onNodeStateChanged(QTreeWidgetItem* item, int column);

    void onPoleSlotEdited(const QString& text);

    void onPoleInitialized();

    void onPoleTypeChanged(int index);

    void onPoleSelected(int index);

    void onConfirmBtnClicked();

    void onBaseTypeChanged(int index);

    void onBaseSizeChanged(int index);

public:
    PoleAddPanel(QWidget* parent = nullptr);
    ~PoleAddPanel() override;

signals:
    void poleSubmitted(Pole* pole);

private:
    Ui::PoleAddPanel* ui;
    QDoubleValidator* doubleValidator;

    QMetaObject::Connection stateChangedConnection;
    QMetaObject::Connection texClickedConnection;
    QMetaObject::Connection texDoubleClickedConnection;

    QString dirPath;

    QStringList cubeSizes;
    QStringList cylinderSizes;

    QVector<BaseInfo> bases;
};