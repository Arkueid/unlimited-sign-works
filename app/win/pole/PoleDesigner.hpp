#pragma once

#include <ui_PoleDesigner.h>
#include <QTimer>

#include <base/Cube.hpp>
#include <base/Cylinder.hpp>
#include <base/Stiffener.hpp>
#include <base/RoundedCube.hpp>
#include <base/RoundedRegTriPrism.hpp>

#include <QDoubleValidator>
#include <QMetaObject>

class PoleDesigner: public QWidget
{
	Ui::PoleDesigner* ui;

	QTimer timer;

	Cube* cube;
	Cylinder* cylinder;
	Stiffener* stiffener;
	RoundedCube* roundedCube;
	RoundedRegTriPrism* rrTriPrism;

	QMetaObject::Connection connection;

	QDoubleValidator* doubleValidator;

	bool sceneInitialized;

	void bindPart(Part* part);

	void bindCube(Part* part);
	void bindCylinder(Part* part);
	void bindStiffener(Part* part);
	void bindRoundedCube(Part* part);
	void bindRoundedRegTriPrism(Part* part);

	void hidePanels();

	void initializeLineEdits();

	void bindTransforms(Part* part);

	void bindTextures(Part* part, const QVector<QComboBox*>& comboBoxes, const QVector<QCheckBox *> &checkBoxes);

	void refreshMesh(Part* part, Mesh* mesh);

private slots:
	void onDrawableAdded(PartDrawable* d);

	void onAddBtnClicked();

	void onRemoveBtnClicked();

	void onCopyBtnClicked();

	void onDrawableItemSelected(QListWidgetItem* item);

	void onPoleNameEdited();

	void onPoleTypeChanged(int index);

	void onSaveBtnClicked();

	void onTextureBaseUpdated();

	void onSceneInitialized();

public:
	explicit PoleDesigner(QWidget* parent = nullptr, const QString& poleJsonPath = "");
	~PoleDesigner() override;

	QLineEdit* getPoleNameLineEdit();

};

