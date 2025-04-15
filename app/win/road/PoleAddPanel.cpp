#include "PoleAddPanel.hpp"

#include <base/Cube.hpp>
#include <base/Cylinder.hpp>
#include <base/Stiffener.hpp>
#include <base/RoundedCube.hpp>
#include <base/RoundedRegTriPrism.hpp>

#include <utils/TextureManager.hpp>
#include <win/road/TextureSetPanel.hpp>
#include <utils/PoleJsonSerializer.hpp>

#include <QDir>

PoleAddPanel::PoleAddPanel(QWidget *parent) : QDialog(parent), ui(new Ui::PoleAddPanel), doubleValidator(new QDoubleValidator)
{
	ui->setupUi(this);

	resize(1200, 600);

	ui->poleScene->loadPole("");

	ui->poleSlot->setText(ui->poleScene->getPoleName());

	ui->splitter->setStretchFactor(0, 1);
	ui->splitter->setStretchFactor(1, 5);
	ui->splitter->setStretchFactor(2, 1);

	setAttribute(Qt::WA_DeleteOnClose);
	setWindowModality(Qt::WindowModality::ApplicationModal);

	initializeLineEdits();

	connect(ui->poleScene, &PoleScene::initialized, this, &PoleAddPanel::onPoleInitialized);

	connect(ui->poleSlot, &QLineEdit::textEdited, this, &PoleAddPanel::onPoleSlotEdited);

	connect(ui->nodeTree, &QTreeWidget::itemClicked, this, &PoleAddPanel::onNodeClicked);

	connect(ui->poleTypeSelector, &QComboBox::currentIndexChanged, this, &PoleAddPanel::onPoleTypeChanged);

	onPoleTypeChanged(0);

	hidePanels();
}

PoleAddPanel::~PoleAddPanel()
{
	delete ui;
	delete doubleValidator;
}

void PoleAddPanel::initNodeTree()
{
	ui->nodeTree->disconnect(stateChangedConnection);

	ui->nodeTree->clear();
	QTreeWidgetItem *root = new QTreeWidgetItem(ui->nodeTree);
	Pole *pole = ui->poleScene->getPole();
	root->setText(0, pole->getName().c_str());

	root->setCheckState(0, Qt::CheckState::Checked);

	const std::vector<Part *> &parts = pole->getParts();

	int index = 0;
	for (auto &part : parts)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(root);
		item->setCheckState(0, Qt::CheckState::Checked);
		item->setText(0, part->getName().c_str());
		item->setData(0, Qt::UserRole, QVariant::fromValue(ui->poleScene->getDrawable(index++)));
	}

	ui->nodeTree->expandAll();

	stateChangedConnection = connect(ui->nodeTree, &QTreeWidget::itemChanged, this, &PoleAddPanel::onNodeStateChanged);
}

void PoleAddPanel::onNodeClicked(QTreeWidgetItem *item, int column)
{
	// qDebug() << "Node clicked";
	if (item->parent() == nullptr)
	{
		return;
	}

	PartDrawable *drawable = item->data(0, Qt::UserRole).value<PartDrawable *>();
	Part *part = drawable->getPart();
	bindPart(part);
	ui->poleScene->selectPart(drawable);
}

void PoleAddPanel::onNodeStateChanged(QTreeWidgetItem *item, int column)
{
	// qDebug() << "onNodeStateChanged";
	if (item->parent() == nullptr)
	{
		for (int i = 0; i < ui->nodeTree->topLevelItemCount(); ++i)
		{
			QTreeWidgetItem *item = ui->nodeTree->topLevelItem(i);
			for (int j = 0; j < item->childCount(); ++j)
			{
				QTreeWidgetItem *child = item->child(j);
				child->setCheckState(0, item->checkState(0));
			}
		}
		return;
	}
	else
	{
		PartDrawable *drawable = item->data(0, Qt::UserRole).value<PartDrawable *>();
		drawable->setVisible(item->checkState(0) == Qt::Checked);
	}

	ui->poleScene->update();
}

void PoleAddPanel::onPoleSlotEdited(const QString &text)
{
	QTreeWidgetItem *item = ui->nodeTree->topLevelItem(0);
	item->setText(0, text);
	ui->poleScene->setPoleName(text);
}

void PoleAddPanel::onPoleInitialized()
{
	initNodeTree();
	initTexturePanel();
}

void PoleAddPanel::onPoleTypeChanged(int index)
{
	QDir dir = QDir::current();
	dir.cdUp();
	dir.cdUp();
	dirPath = dir.absoluteFilePath("assets/poles");
	switch (index)
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
	ui->poleSelector->disconnect();
	ui->poleSelector->clear();
	dir = QDir(dirPath);
	for (auto& f: dir.entryList(QDir::Files))
	{
		if (f.endsWith(".json"))
		{
			ui->poleSelector->addItem(f.replace(".json", ""));
		}
	}

	ui->poleSelector->setCurrentIndex(-1);
	connect(ui->poleSelector, &QComboBox::currentIndexChanged, this, &PoleAddPanel::onPoleSelected);

	ui->poleScene->changePole(nullptr);
	ui->textureTable->setRowCount(0);
	ui->nodeTree->clear();
}

void PoleAddPanel::onPoleSelected(int index)
{
	QDir dir(dirPath);

	Pole* pole = PoleJsonSerializer::fromJsonFile(dir.absoluteFilePath(ui->poleSelector->currentText() + ".json"));

	ui->poleSlot->setText(pole->getName().c_str());

	ui->poleScene->changePole(pole);
}

void PoleAddPanel::bindPart(Part *part)
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

void PoleAddPanel::bindCube(Part *part)
{
	ui->pages->setCurrentWidget(ui->cubePanel);

	// 绑定基本数据
	ui->cubeName->setText(part->getName().c_str());
	Cube *cube = (Cube *)part->getMesh();

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
				QTreeWidgetItem *item = ui->nodeTree->currentItem();
				item->setText(0, text);
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

	// 变换
	bindTransforms(part);
}

void PoleAddPanel::bindCylinder(Part *part)
{
	ui->pages->setCurrentWidget(ui->cylinderPanel);

	ui->cylinderName->setText(part->getName().c_str());
	Cylinder *cylinder = (Cylinder *)part->getMesh();

	ui->cylinderD->setText(QString::number(cylinder->getD()));
	ui->cylinderH->setText(QString::number(cylinder->getH()));

	// 事件绑定
	ui->cylinderName->disconnect();
	connect(ui->cylinderName, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setName(text.toStdString());
				QTreeWidgetItem *item = ui->nodeTree->currentItem();
				item->setText(0, text);
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

	bindTransforms(part);
}

void PoleAddPanel::bindStiffener(Part *part)
{
	ui->pages->setCurrentWidget(ui->stiffenerPanel);

	// 绑定数据
	ui->stiffenerName->setText(part->getName().c_str());
	Stiffener *stiffener = (Stiffener *)part->getMesh();
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
				QTreeWidgetItem *item = ui->nodeTree->currentItem();
				item->setText(0, text);
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
	// 变换
	bindTransforms(part);
}

void PoleAddPanel::bindRoundedCube(Part *part)
{
	ui->pages->setCurrentWidget(ui->rCubePanel);

	RoundedCube *roundedCube = (RoundedCube *)part->getMesh();

	// 绑定数据
	ui->rCubeName->setText(part->getName().c_str());
	ui->rCubeA->setText(QString::number(roundedCube->getA()));
	ui->rCubeB->setText(QString::number(roundedCube->getB()));
	ui->rCubeC->setText(QString::number(roundedCube->getC()));
	ui->rCubeRadius->setText(QString::number(roundedCube->getRadius()));

	// 绑定信号
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

void PoleAddPanel::bindRoundedRegTriPrism(Part *part)
{
	ui->pages->setCurrentWidget(ui->rRegTriPrismPanel);

	// 绑定数据
	ui->rRegTriPrismName->setText(part->getName().c_str());
	RoundedRegTriPrism *roundedRegTriPrism = (RoundedRegTriPrism *)part->getMesh();

	ui->rRegTriPrismA->setText(QString::number(roundedRegTriPrism->getA()));
	ui->rRegTriPrismH->setText(QString::number(roundedRegTriPrism->getH()));
	ui->rRegTriPrismRadius->setText(QString::number(roundedRegTriPrism->getRadius()));

	// 绑定信号
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

	// 变换
	bindTransforms(part);
}

void PoleAddPanel::hidePanels()
{
	ui->transformPanel->setVisible(false);
	ui->pages->setCurrentWidget(ui->empty);
}

void PoleAddPanel::initializeLineEdits()
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

void PoleAddPanel::bindTransforms(Part *part)
{
	PartDrawable *drawable = ui->nodeTree->currentItem()->data(0, Qt::UserRole).value<PartDrawable *>();
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

void PoleAddPanel::refreshMesh(Part *part, Mesh *mesh)
{
	PartDrawable *drawable = ui->nodeTree->currentItem()->data(0, Qt::UserRole).value<PartDrawable *>();
	ui->poleScene->unbindPart(drawable);
	part->setMesh(mesh);
	ui->poleScene->updatePart(drawable);
	ui->poleScene->update();
}

void PoleAddPanel::initTexturePanel()
{
	ui->textureTable->disconnect(texClickedConnection);
	ui->textureTable->disconnect(texDoubleClickedConnection);
	ui->textureTable->setRowCount(0);

	const std::vector<Part *> parts = ui->poleScene->getPole()->getParts();

	int partIndex = 0;
	for (auto &part : parts)
	{
		const int drawableCount = part->getDrawableCount();
		const std::vector<bool> textureEdits = part->getTextureEdits();
		for (int i = 0; i < drawableCount; i++)
		{
			if (textureEdits[i])
			{
				part->setTextureRef(i, -2);

				const int row = ui->textureTable->rowCount();
				ui->textureTable->insertRow(row);

				QTableWidgetItem *item = new QTableWidgetItem("未设置");
				ui->textureTable->setItem(row, 0, item);
				item->setData(Qt::UserRole, QVariant::fromValue(part));
				item->setData(Qt::UserRole + 1, QVariant::fromValue(i));
				item->setData(Qt::UserRole + 2, QVariant::fromValue(partIndex));
			}
		}
		partIndex++;
	}
	texClickedConnection = connect(ui->textureTable, &QTableWidget::itemClicked,
		[=](QTableWidgetItem *item)
		{
			PartDrawable *partDrawable = ui->poleScene->getDrawable(item->data(Qt::UserRole + 2).toInt());
			ui->poleScene->selectPart(partDrawable);
		}
	);

	texDoubleClickedConnection = connect(ui->textureTable, &QTableWidget::itemDoubleClicked,
			[this](QTableWidgetItem *item)
			{
				if (item->column() == 0)
				{
					Part *part = item->data(Qt::UserRole).value<Part *>();
					int index = item->data(Qt::UserRole + 1).value<int>();
					TextureSetPanel *textureSetPanel = new TextureSetPanel(part, index);
					connect(textureSetPanel, &TextureSetPanel::success, [this, item](const QString &textureName)
							{ 
								item->setText(textureName);
								ui->poleScene->update(); 
							});
					textureSetPanel->show();
				}
			});
}