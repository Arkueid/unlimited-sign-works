#include <win/pole/PoleDesigner.hpp>
#include <win/list/DrawableListItem.hpp>
#include <utils/TextureManager.hpp>
#include <win/pole/AddPanel.hpp>
#include <utils/PoleJsonSerializer.hpp>

#include <QFileDialog>
#include <QMenu>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QMessageBox>

PoleDesigner::PoleDesigner(QWidget *parent, const QString &poleJsonPath) : QWidget(parent),
																		   ui(new Ui::PoleDesigner),
																		   cube(nullptr),
																		   cylinder(nullptr),
																		   stiffener(nullptr),
																		   roundedCube(nullptr),
																		   rrTriPrism(nullptr),
																		   doubleValidator(new QDoubleValidator(this)),
																		   sceneInitialized(false)
{
	ui->setupUi(this);

	ui->poleScene->loadPole(poleJsonPath);

	ui->leftSplitter->setStretchFactor(0, 1);
	ui->leftSplitter->setStretchFactor(1, 8);
	ui->rightSplitter->setStretchFactor(0, 8);
	ui->rightSplitter->setStretchFactor(1, 1);

	resize(1280, 720);

	initializeLineEdits();

	connect(ui->poleScene, &PoleScene::drawableAdded, this, &PoleDesigner::onDrawableAdded);

	connect(ui->addBtn, &QPushButton::clicked, this, &PoleDesigner::onAddBtnClicked);

	connect(ui->removeBtn, &QPushButton::clicked, this, &PoleDesigner::onRemoveBtnClicked);

	connect(ui->copyBtn, &QPushButton::clicked, this, &PoleDesigner::onCopyBtnClicked);

	connect(ui->drawableList, &QListWidget::itemClicked, this, &PoleDesigner::onDrawableItemSelected);

	ui->poleName->setText(ui->poleScene->getPoleName());
	connect(ui->poleName, &QLineEdit::textEdited, this, &PoleDesigner::onPoleNameEdited);

	ui->poleType->setCurrentIndex(ui->poleScene->getPole()->getType());
	connect(ui->poleType, &QComboBox::currentIndexChanged, this, &PoleDesigner::onPoleTypeChanged);

	connect(ui->saveBtn, &QPushButton::clicked, this, &PoleDesigner::onSaveBtnClicked);

	connect(ui->poleScene, &PoleScene::initialized, this, &PoleDesigner::onSceneInitialized);

	// 需要手动解除
	connection = connect(TextureManager::getTexturesSignals(), &TextureSignals::texturesUpdated,
						 this, &PoleDesigner::onTextureBaseUpdated);

	ui->paramPanel->setCurrentWidget(ui->empty);
	ui->transformPanel->setVisible(false);
}

PoleDesigner::~PoleDesigner()
{
	disconnect(connection);
	delete ui;
}

QLineEdit *PoleDesigner::getPoleNameLineEdit()
{
	return ui->poleName;
}

void PoleDesigner::bindPart(Part *part)
{
	switch (part->getType())
	{
	case MeshType::MeshType_Cube:
		bindCube(part);
		break;
	case MeshType::MeshType_Cylinder:
		bindCylinder(part);
		break;
	case MeshType::MeshType_Stiffener:
		bindStiffener(part);
		break;
	case MeshType::MeshType_RoundedCube:
		bindRoundedCube(part);
		break;
	case MeshType::MeshType_RoundedRegTriPrism:
		bindRoundedRegTriPrism(part);
		break;
	default:
		break;
	}

	ui->transformPanel->setVisible(true);
}

void PoleDesigner::bindCube(Part *part)
{
	ui->paramPanel->setCurrentWidget(ui->cubePanel);

	ui->cubeIsPoleBase->disconnect();
	ui->cubeIsPoleBase->setChecked(part->isPoleBase());
	connect(ui->cubeIsPoleBase, &QCheckBox::checkStateChanged,
			[=](int state)
			{
				part->setPoleBase(state == Qt::Checked);
			});

	// 绑定基本数据
	ui->cubeName->setText(part->getName().c_str());
	cube = (Cube *)part->getMesh();

	ui->cubeA->setText(QString::number(cube->getA()));
	ui->cubeB->setText(QString::number(cube->getB()));
	ui->cubeC->setText(QString::number(cube->getC()));

	// 事件绑定
	// 名称
	ui->cubeName->disconnect();
	connect(ui->cubeName, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setName(text.toStdString());
				QListWidgetItem *item = ui->drawableList->currentItem();
				DrawableListItem *widget = reinterpret_cast<DrawableListItem *>(ui->drawableList->itemWidget(item));
				widget->getName()->setText(text);
			});

	// 形状参数
	ui->cubeA->disconnect();
	connect(ui->cubeA, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Cube *oldCube = (Cube *)part->getMesh();
				Cube *newCube = Cube::create(text.toFloat(), oldCube->getB(), oldCube->getC());
				refreshMesh(part, newCube);
			});
	ui->cubeB->disconnect();
	connect(ui->cubeB, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Cube *oldCube = (Cube *)part->getMesh();
				Cube *newCube = Cube::create(oldCube->getA(), text.toFloat(), oldCube->getC());
				refreshMesh(part, newCube);
			});
	ui->cubeC->disconnect();
	connect(ui->cubeC, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Cube *oldCube = (Cube *)part->getMesh();
				Cube *newCube = Cube::create(oldCube->getA(), oldCube->getB(), text.toFloat());
				refreshMesh(part, newCube);
			});

	// 纹理
	bindTextures(part,
				 {ui->cubeTexFront, ui->cubeTexLeft, ui->cubeTexBack, ui->cubeTexRight, ui->cubeTexBottom, ui->cubeTexTop},
				 {ui->cubeTexFrontEditable, ui->cubeTexLeftEditable, ui->cubeTexBackEditable, ui->cubeTexRightEditable, ui->cubeTexBottomEditable, ui->cubeTexTopEditable});

	// 变换
	bindTransforms(part);
}

void PoleDesigner::bindCylinder(Part *part)
{
	ui->paramPanel->setCurrentWidget(ui->cylinderPanel);

	ui->cylinderIsPoleBase->disconnect();
	ui->cylinderIsPoleBase->setChecked(part->isPoleBase());
	connect(ui->cylinderIsPoleBase, &QCheckBox::checkStateChanged,
			[=](int state)
			{
				part->setPoleBase(state == Qt::Checked);
			});

	ui->cylinderName->setText(part->getName().c_str());
	cylinder = (Cylinder *)part->getMesh();

	ui->cylinderD->setText(QString::number(cylinder->getD()));
	ui->cylinderH->setText(QString::number(cylinder->getH()));

	// 事件绑定
	ui->cylinderName->disconnect();
	connect(ui->cylinderName, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setName(text.toStdString());
				QListWidgetItem *item = ui->drawableList->currentItem();
				DrawableListItem *widget = reinterpret_cast<DrawableListItem *>(ui->drawableList->itemWidget(item));
				widget->getName()->setText(text);
			});
	ui->cylinderD->disconnect();
	connect(ui->cylinderD, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Cylinder *oldCylinder = (Cylinder *)part->getMesh();
				Cylinder *newCylinder = Cylinder::create(text.toFloat(), oldCylinder->getH());
				refreshMesh(part, newCylinder);
			});
	ui->cylinderH->disconnect();
	connect(ui->cylinderH, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Cylinder *oldCylinder = (Cylinder *)part->getMesh();
				Cylinder *newCylinder = Cylinder::create(oldCylinder->getD(), text.toFloat());
				refreshMesh(part, newCylinder);
			});

	bindTextures(part,
				 {ui->cylinderTexTop, ui->cylinderTexBottom, ui->cylinderTexSide},
				 {ui->cylinderTexTopEditable, ui->cylinderTexBottomEditable, ui->cylinderTexSideEditable});

	bindTransforms(part);
}

void PoleDesigner::bindStiffener(Part *part)
{
	ui->paramPanel->setCurrentWidget(ui->stiffenerPanel);

	// 绑定数据
	ui->stiffenerName->setText(part->getName().c_str());
	stiffener = (Stiffener *)part->getMesh();
	ui->stiffenerA->setText(QString::number(stiffener->getA()));
	ui->stiffenerB->setText(QString::number(stiffener->getB()));
	ui->stiffenerC->setText(QString::number(stiffener->getC()));
	ui->stiffenerH1->setText(QString::number(stiffener->getH1()));
	ui->stiffenerH2->setText(QString::number(stiffener->getH2()));
	ui->stiffenerHOff->setText(QString::number(stiffener->getOffsetH()));
	ui->stiffenerVOff->setText(QString::number(stiffener->getOffsetV()));

	// 事件绑定
	ui->stiffenerName->disconnect();
	connect(ui->stiffenerName, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setName(text.toStdString());
				QListWidgetItem *item = ui->drawableList->currentItem();
				DrawableListItem *widget = reinterpret_cast<DrawableListItem *>(ui->drawableList->itemWidget(item));
				widget->getName()->setText(text);
			});
	ui->stiffenerA->disconnect();
	connect(ui->stiffenerA, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Stiffener *oldStiffener = (Stiffener *)part->getMesh();
				Stiffener *newStiffener = Stiffener::create(text.toFloat(), oldStiffener->getB(), oldStiffener->getH1(), oldStiffener->getH2(), oldStiffener->getC(), oldStiffener->getOffsetH(), oldStiffener->getOffsetV());
				refreshMesh(part, newStiffener);
			});
	ui->stiffenerB->disconnect();
	connect(ui->stiffenerB, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Stiffener *oldStiffener = (Stiffener *)part->getMesh();
				Stiffener *newStiffener = Stiffener::create(oldStiffener->getA(), text.toFloat(), oldStiffener->getH1(), oldStiffener->getH2(), oldStiffener->getC(), oldStiffener->getOffsetH(), oldStiffener->getOffsetV());
				refreshMesh(part, newStiffener);
			});
	ui->stiffenerH1->disconnect();
	connect(ui->stiffenerH1, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Stiffener *oldStiffener = (Stiffener *)part->getMesh();
				Stiffener *newStiffener = Stiffener::create(oldStiffener->getA(), oldStiffener->getB(), text.toFloat(), oldStiffener->getH2(), oldStiffener->getC(), oldStiffener->getOffsetH(), oldStiffener->getOffsetV());
				refreshMesh(part, newStiffener);
			});
	ui->stiffenerH2->disconnect();
	connect(ui->stiffenerH2, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Stiffener *oldStiffener = (Stiffener *)part->getMesh();
				Stiffener *newStiffener = Stiffener::create(oldStiffener->getA(), oldStiffener->getB(), oldStiffener->getH1(), text.toFloat(), oldStiffener->getC(), oldStiffener->getOffsetH(), oldStiffener->getOffsetV());
				refreshMesh(part, newStiffener);
			});
	ui->stiffenerC->disconnect();
	connect(ui->stiffenerC, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Stiffener *oldStiffener = (Stiffener *)part->getMesh();
				Stiffener *newStiffener = Stiffener::create(oldStiffener->getA(), oldStiffener->getB(), oldStiffener->getH1(), oldStiffener->getH2(), text.toFloat(), oldStiffener->getOffsetH(), oldStiffener->getOffsetV());
				refreshMesh(part, newStiffener);
			});
	ui->stiffenerHOff->disconnect();
	connect(ui->stiffenerHOff, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Stiffener *oldStiffener = (Stiffener *)part->getMesh();
				Stiffener *newStiffener = Stiffener::create(oldStiffener->getA(), oldStiffener->getB(), oldStiffener->getH1(), oldStiffener->getH2(), oldStiffener->getC(), text.toFloat(), oldStiffener->getOffsetV());
				refreshMesh(part, newStiffener);
			});
	ui->stiffenerVOff->disconnect();
	connect(ui->stiffenerVOff, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Stiffener *oldStiffener = (Stiffener *)part->getMesh();
				Stiffener *newStiffener = Stiffener::create(oldStiffener->getA(), oldStiffener->getB(), oldStiffener->getH1(), oldStiffener->getH2(), oldStiffener->getC(), oldStiffener->getOffsetH(), text.toFloat());
				refreshMesh(part, newStiffener);
			});

	// 纹理
	bindTextures(part,
				 {ui->stiffenerTexFront,
				  ui->stiffenerTexBack,
				  ui->stiffenerTexRight,
				  ui->stiffenerTexBottom,
				  ui->stiffenerTexSlope,
				  ui->stiffenerTexTop,
				  ui->stiffenerTexLeft},
				 {ui->stiffenerTexFrontEditable,
				  ui->stiffenerTexBackEditable,
				  ui->stiffenerTexRightEditable,
				  ui->stiffenerTexBottomEditable,
				  ui->stiffenerTexSlopeEditable,
				  ui->stiffenerTexTopEditable,
				  ui->stiffenerTexLeftEditable});
	// 变换
	bindTransforms(part);
}

void PoleDesigner::bindRoundedCube(Part *part)
{
	ui->paramPanel->setCurrentWidget(ui->rCubePanel);

	RoundedCube *roundedCube = (RoundedCube *)part->getMesh();

	// 绑定数据
	ui->rCubeName->setText(part->getName().c_str());
	ui->rCubeA->setText(QString::number(roundedCube->getA()));
	ui->rCubeB->setText(QString::number(roundedCube->getB()));
	ui->rCubeC->setText(QString::number(roundedCube->getC()));
	ui->rCubeRadius->setText(QString::number(roundedCube->getRadius()));

	// 纹理
	bindTextures(part,
				 {ui->rCubeTexTop,
				  ui->rCubeTexBottom,
				  ui->rCubeTexSide},
				 {ui->rCubeTexTopEditable,
				  ui->rCubeTexBottomEditable,
				  ui->rCubeTexSideEditable});

	// 绑定信号
	ui->rCubeName->disconnect();
	connect(ui->rCubeName, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setName(text.toStdString());
				QListWidgetItem *item = ui->drawableList->currentItem();
				DrawableListItem *widget = reinterpret_cast<DrawableListItem *>(ui->drawableList->itemWidget(item));
				widget->getName()->setText(text);
			});
	// 基础参数
	ui->rCubeA->disconnect();
	connect(ui->rCubeA, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				RoundedCube *oldRCube = (RoundedCube *)part->getMesh();
				RoundedCube *newRCube = RoundedCube::create(text.toFloat(), oldRCube->getB(), oldRCube->getC(), oldRCube->getRadius());
				refreshMesh(part, newRCube);
			});
	ui->rCubeB->disconnect();
	connect(ui->rCubeB, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				RoundedCube *oldRCube = (RoundedCube *)part->getMesh();
				RoundedCube *newRCube = RoundedCube::create(oldRCube->getA(), text.toFloat(), oldRCube->getC(), oldRCube->getRadius());
				refreshMesh(part, newRCube);
			});
	ui->rCubeC->disconnect();
	connect(ui->rCubeC, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				RoundedCube *oldRCube = (RoundedCube *)part->getMesh();
				RoundedCube *newRCube = RoundedCube::create(oldRCube->getA(), oldRCube->getB(), text.toFloat(), oldRCube->getRadius());
				refreshMesh(part, newRCube);
			});
	ui->rCubeRadius->disconnect();
	connect(ui->rCubeRadius, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				RoundedCube *oldRCube = (RoundedCube *)part->getMesh();
				RoundedCube *newRCube = RoundedCube::create(oldRCube->getA(), oldRCube->getB(), oldRCube->getC(), text.toFloat());
				refreshMesh(part, newRCube);
			});

	// 变换
	bindTransforms(part);
}

void PoleDesigner::bindRoundedRegTriPrism(Part *part)
{
	ui->paramPanel->setCurrentWidget(ui->rRegTriPrismPanel);

	// 绑定数据
	ui->rRegTriPrismName->setText(part->getName().c_str());
	RoundedRegTriPrism *roundedRegTriPrism = (RoundedRegTriPrism *)part->getMesh();

	ui->rRegTriPrismA->setText(QString::number(roundedRegTriPrism->getA()));
	ui->rRegTriPrismH->setText(QString::number(roundedRegTriPrism->getH()));
	ui->rRegTriPrismRadius->setText(QString::number(roundedRegTriPrism->getRadius()));

	// 绑定信号
	ui->rRegTriPrismName->disconnect();
	connect(ui->rRegTriPrismName, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setName(text.toStdString());
				QListWidgetItem *item = ui->drawableList->currentItem();
				DrawableListItem *widget = reinterpret_cast<DrawableListItem *>(ui->drawableList->itemWidget(item));
				widget->getName()->setText(text);
			});
	// 基础参数
	ui->rRegTriPrismA->disconnect();
	connect(ui->rRegTriPrismA, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				RoundedRegTriPrism *oldRrtp = (RoundedRegTriPrism *)part->getMesh();
				RoundedRegTriPrism *newRrtp = RoundedRegTriPrism::create(text.toFloat(), oldRrtp->getH(), oldRrtp->getRadius());
				refreshMesh(part, newRrtp);
			});
	ui->rRegTriPrismH->disconnect();
	connect(ui->rRegTriPrismH, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				RoundedRegTriPrism *oldRrtp = (RoundedRegTriPrism *)part->getMesh();
				RoundedRegTriPrism *newRrtp = RoundedRegTriPrism::create(oldRrtp->getA(), text.toFloat(), oldRrtp->getRadius());
				refreshMesh(part, newRrtp);
			});
	ui->rRegTriPrismRadius->disconnect();
	connect(ui->rRegTriPrismRadius, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				RoundedRegTriPrism *oldRrtp = (RoundedRegTriPrism *)part->getMesh();
				RoundedRegTriPrism *newRrtp = RoundedRegTriPrism::create(oldRrtp->getA(), oldRrtp->getH(), text.toFloat());
				refreshMesh(part, newRrtp);
			});

	// 纹理
	bindTextures(part,
				 {ui->rRegTriPrismTexTop,
				  ui->rRegTriPrismTexBottom,
				  ui->rRegTriPrismTexSide},
				 {ui->rRegTriPrismTexTopEditable,
				  ui->rRegTriPrismTexBottomEditable,
				  ui->rRegTriPrismTexSideEditable});

	// 变换
	bindTransforms(part);
}

void PoleDesigner::hidePanels()
{
	ui->transformPanel->setVisible(false);
	ui->paramPanel->setCurrentWidget(ui->empty);
}

void PoleDesigner::initializeLineEdits()
{
	doubleValidator->setRange(-1000000000, 1000000000, 5);
	doubleValidator->setNotation(QDoubleValidator::StandardNotation);

	ui->originX->setValidator(doubleValidator);
	ui->originY->setValidator(doubleValidator);
	ui->originZ->setValidator(doubleValidator);
	ui->rotationX->setValidator(doubleValidator);
	ui->rotationY->setValidator(doubleValidator);
	ui->rotationZ->setValidator(doubleValidator);

	ui->cubeA->setValidator(doubleValidator);
	ui->cubeB->setValidator(doubleValidator);
	ui->cubeC->setValidator(doubleValidator);

	ui->cylinderD->setValidator(doubleValidator);
	ui->cylinderH->setValidator(doubleValidator);

	ui->rCubeA->setValidator(doubleValidator);
	ui->rCubeB->setValidator(doubleValidator);
	ui->rCubeC->setValidator(doubleValidator);
	ui->rCubeRadius->setValidator(doubleValidator);

	ui->rRegTriPrismA->setValidator(doubleValidator);
	ui->rRegTriPrismH->setValidator(doubleValidator);
	ui->rRegTriPrismRadius->setValidator(doubleValidator);

	ui->stiffenerA->setValidator(doubleValidator);
	ui->stiffenerB->setValidator(doubleValidator);
	ui->stiffenerC->setValidator(doubleValidator);
	ui->stiffenerH1->setValidator(doubleValidator);
	ui->stiffenerH2->setValidator(doubleValidator);
	ui->stiffenerHOff->setValidator(doubleValidator);
	ui->stiffenerVOff->setValidator(doubleValidator);
}

void PoleDesigner::bindTransforms(Part *part)
{
	PartDrawable *drawable = ui->drawableList->currentItem()->data(Qt::UserRole).value<PartDrawable *>();
	ui->originX->setText(QString::number(part->getOriginX()));
	ui->originY->setText(QString::number(part->getOriginY()));
	ui->originZ->setText(QString::number(part->getOriginZ()));

	ui->rotationX->setText(QString::number(part->getRotationX()));
	ui->rotationY->setText(QString::number(part->getRotationY()));
	ui->rotationZ->setText(QString::number(part->getRotationZ()));

	ui->originX->disconnect();
	connect(ui->originX, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setOriginX(text.toFloat());
				ui->poleScene->selectPart(drawable);
				ui->poleScene->update();
			});
	ui->originY->disconnect();
	connect(ui->originY, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setOriginY(text.toFloat());
				ui->poleScene->selectPart(drawable);
				ui->poleScene->update();
			});
	ui->originZ->disconnect();
	connect(ui->originZ, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setOriginZ(text.toFloat());
				ui->poleScene->selectPart(drawable);
				ui->poleScene->update();
			});
	ui->rotationX->disconnect();
	connect(ui->rotationX, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setRotationX(text.toFloat());
				ui->poleScene->selectPart(drawable);
				ui->poleScene->update();
			});
	ui->rotationY->disconnect();
	connect(ui->rotationY, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setRotationY(text.toFloat());
				ui->poleScene->selectPart(drawable);
				ui->poleScene->update();
			});
	ui->rotationZ->disconnect();
	connect(ui->rotationZ, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setRotationZ(text.toFloat());
				ui->poleScene->selectPart(drawable);
				ui->poleScene->update();
			});
}

void PoleDesigner::bindTextures(Part *part, const QVector<QComboBox *> &comboBoxes, const QVector<QCheckBox *> &checkBoxes)
{
	const int size = comboBoxes.size();
	const QMap<int, QString> &texturePaths = TextureManager::getTexturePaths();
	for (int i = 0; i < size; i++)
	{
		QComboBox *comboBox = comboBoxes[i];
		const int cidx = part->getTextureRefs()[i];
		comboBox->disconnect();
		comboBox->clear();
		int idx = -1;
		int count = 0;
		for (auto it = texturePaths.begin(); it != texturePaths.end(); ++it)
		{
			comboBox->addItem(it.value(), it.key());
			if (it.key() == cidx)
			{
				idx = count;
			}
			count++;
		}
		comboBox->setCurrentIndex(idx);
		connect(comboBox, &QComboBox::currentIndexChanged,
				[=](int index)
				{
					part->setTextureRef(i, comboBox->itemData(index).toInt());
					ui->poleScene->update();
				});

		QCheckBox *checkBox = checkBoxes[i];
		const bool e = part->getTextureEdits()[i];
		checkBox->disconnect();
		checkBox->setChecked(e);
		connect(checkBox, &QCheckBox::checkStateChanged,
				[=](Qt::CheckState state)
				{
					part->setTextureEdit(i, state == Qt::Checked);
					ui->poleScene->update();
				});
	}
}

void PoleDesigner::refreshMesh(Part *part, Mesh *mesh)
{
	PartDrawable *drawable = ui->drawableList->currentItem()->data(Qt::UserRole).value<PartDrawable *>();
	ui->poleScene->unbindPart(drawable);
	part->setMesh(mesh);
	ui->poleScene->updatePart(drawable);
	ui->poleScene->update();
}

void PoleDesigner::onDrawableAdded(PartDrawable *d)
{
	QListWidgetItem *item = new QListWidgetItem(ui->drawableList);
	DrawableListItem *widget = new DrawableListItem(ui->drawableList);
	widget->getName()->setText(d->getName());
	item->setSizeHint(widget->size());
	item->setData(Qt::UserRole, QVariant::fromValue(d));
	ui->drawableList->addItem(item);
	ui->drawableList->setItemWidget(item, widget);

	QCheckBox *chkbx = widget->getCheckBox();
	chkbx->setChecked(true);
	connect(chkbx, &QCheckBox::checkStateChanged,
			[=](Qt::CheckState state)
			{
				if (state == Qt::Checked)
				{
					d->setVisible(true);
				}
				else
				{
					d->setVisible(false);
				}
				ui->poleScene->update();
			});

	if (sceneInitialized) // 初始化时不需要绑定，提高加载速度
	{
		// qDebug() << "Binding part: " << d->getPart()->getName();
		ui->drawableList->setCurrentItem(item);
		bindPart(d->getPart());
		ui->poleScene->selectPart(d);
	}
}

void PoleDesigner::onAddBtnClicked()
{
	AddPanel *panel = new AddPanel(ui->poleScene);
	panel->show();
}

void PoleDesigner::onRemoveBtnClicked()
{
	if (ui->drawableList->currentRow() >= 0)
	{
		// qDebug() << "Removing drawable: " << ui->drawableList->currentRow();
		QListWidgetItem *item = ui->drawableList->takeItem(ui->drawableList->currentRow());
		ui->poleScene->removeDrawable(item->data(Qt::UserRole).value<PartDrawable *>());
		delete item;
		hidePanels();
		ui->poleScene->unselectPart();
	}
}

void PoleDesigner::onDrawableItemSelected(QListWidgetItem *item)
{
	PartDrawable *drawable = item->data(Qt::UserRole).value<PartDrawable *>();
	Part *part = drawable->getPart();
	bindPart(part);
	ui->poleScene->selectPart(drawable);
}

void PoleDesigner::onPoleNameEdited()
{
	ui->poleScene->setPoleName(ui->poleName->text());
}

void PoleDesigner::onPoleTypeChanged(int index)
{
	ui->poleScene->getPole()->setType(index);
}

void PoleDesigner::onSaveBtnClicked()
{
	QFileDialog dialog(this);
	dialog.setNameFilter("Pole JSON (*.json)");
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setFileMode(QFileDialog::AnyFile);
	QDir dir = QDir::current();
	dir.cdUp();
	dir.cdUp();
	// ../../assets/poles
	int type = ui->poleType->currentIndex();
	QString dirPath = dir.absoluteFilePath("assets/poles");
	switch (type)
	{
	case PoleType_Single:
		dirPath += "/single";
		break;
	case PoleType_Double:
		dirPath += "/double";
		break;
	case PoleType_Gate:
		dirPath += "/gate";
		break;
	case PoleType_SingleArm:
		dirPath += "/single-arm";
		break;
	case PoleType_DoubleArm:
		dirPath += "/double-arm";
		break;
	}
	dialog.setDirectory(dirPath);
	dialog.selectFile(ui->poleScene->getPoleName() + ".json");
	if (dialog.exec())
	{
		QString filePath = dialog.selectedFiles().first();
		// qDebug() << "Saving to: " << filePath;
		PoleJsonSerializer::toJsonFile(ui->poleScene->getPole(), filePath);
	}
}

void PoleDesigner::onTextureBaseUpdated()
{
	QListWidgetItem *item = ui->drawableList->currentItem();
	if (item != nullptr)
	{
		PartDrawable *drawable = item->data(Qt::UserRole).value<PartDrawable *>();
		Part *part = drawable->getPart();
		bindPart(part);
	}
	ui->poleScene->update();
}

void PoleDesigner::onCopyBtnClicked()
{
	QListWidgetItem *item = ui->drawableList->currentItem();
	if (item == nullptr)
	{
		return;
	}

	PartDrawable *drawable = item->data(Qt::UserRole).value<PartDrawable *>();

	QInputDialog dialog(this);
	dialog.setWindowTitle("复制部件");
	dialog.setLabelText("选中部件：" + drawable->getName() + "\n新部件名称:");
	dialog.setTextValue(drawable->getName() + " - 副本");
	dialog.setOkButtonText("确定");
	dialog.setCancelButtonText("取消");
	dialog.adjustSize();
	if (dialog.exec() != QDialog::Accepted)
	{
		return;
	}

	QString name = dialog.textValue();
	if (name.isEmpty())
	{
		QMessageBox::warning(this, "警告", "部件名称不能为空");
		return;
	}

	Part *part = drawable->getPart();
	Mesh *mesh = part->getMesh();
	Part *newPart = new Part(mesh, name.toStdString(),
							 {part->getOriginX(), part->getOriginY(), part->getOriginZ()},
							 {part->getRotationX(), part->getRotationY(), part->getRotationZ()},
							 part->getTextureRefs(), part->getTextureEdits());
							 
	newPart->setPoleBase(part->isPoleBase());

	ui->poleScene->addDrawable(newPart);
}

void PoleDesigner::onSceneInitialized()
{
	sceneInitialized = true;
}