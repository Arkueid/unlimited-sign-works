#pragma once


#include "ui_PoleAddPanel.h"

#include <win/scene/PoleScene.hpp>

#include <QDoubleValidator>
#include <QMetaObject>
#include <QDialog>

#include <QRegularExpressionValidator>

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

    bool checkPoleSlotValid(float& peg);

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
    PoleAddPanel(QWidget* parent = nullptr, bool modify = false);
    ~PoleAddPanel() override;

    void setPole(Pole* pole, const QString& slot, bool isLeft);

signals:
    void poleSubmitted(Pole* pole, 
        float peg, 
        const QString& slot,
        bool isLeft);

private:
    Ui::PoleAddPanel* ui;
    QDoubleValidator* doubleValidator;

    QRegularExpressionValidator* slotValidator;

    QMetaObject::Connection stateChangedConnection;
    QMetaObject::Connection texClickedConnection;
    QMetaObject::Connection texDoubleClickedConnection;

    QString dirPath;

    int poleType;

    QStringList cubeSizes;
    QStringList cylinderSizes;

    QVector<BaseInfo> bases;

    bool textureSelcted;

    bool poleLoaded;
    bool modify;

    bool poleTypeChanged;
    bool baseTypeChanged;
    bool poleChanged;
};