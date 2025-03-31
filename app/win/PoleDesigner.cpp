#include <win/PoleDesigner.hpp>
#include <QListWidgetItem>
#include <win/list/DrawableListItem.hpp>
#include <win/panel/CubePanel.hpp>
#include <QMenu>
#include <utils/TextureManager.hpp>

PoleDesigner::PoleDesigner(QWidget *parent) : QWidget(parent),
											  ui(new Ui::PoleDesigner),
											  cube(nullptr),
											  cylinder(nullptr),
											  stiffener(nullptr),
											  roundedCube(nullptr),
											  rrTriPrism(nullptr),
											  doubleValidator(this)
{
	ui->setupUi(this);
	ui->leftSplitter->setStretchFactor(0, 1);
	ui->leftSplitter->setStretchFactor(1, 8);
	ui->rightSplitter->setStretchFactor(0, 8);
	ui->rightSplitter->setStretchFactor(1, 1);

	resize(1280, 720);

	initializeLineEdits();

	connect(ui->poleScene, &PoleScene::drawableAdded,
			[=](PartDrawable *d)
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
			});

	connect(ui->addBtn, &QPushButton::clicked,
			[=]()
			{
				CubePanel *panel = new CubePanel(ui->poleScene);
				panel->show();
			});

	connect(ui->removeBtn, &QPushButton::clicked,
			[=]()
			{
				if (ui->drawableList->currentRow() >= 0)
				{
					// qDebug() << "Removing drawable: " << ui->drawableList->currentRow();
					QListWidgetItem *item = ui->drawableList->takeItem(ui->drawableList->currentRow());
					ui->poleScene->removeDrawable(item->data(Qt::UserRole).value<PartDrawable *>());
					delete item;
					hidePanels();
				}
			});

	connect(ui->drawableList, &QListWidget::itemClicked,
			[=](QListWidgetItem *item)
			{
				PartDrawable *drawable = item->data(Qt::UserRole).value<PartDrawable *>();
				Part *part = drawable->getPart();
				bindPart(part);
			});

	ui->paramPanel->setCurrentWidget(ui->empty);
	ui->transformPanel->setVisible(false);
}

PoleDesigner::~PoleDesigner()
{
	delete ui;
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

#pragma region 绑定数据
	ui->cubeName->setText(part->getName().c_str());
	cube = (Cube *)part->getMesh();

	ui->cubeA->setText(QString::number(cube->getA()));
	ui->cubeB->setText(QString::number(cube->getB()));
	ui->cubeC->setText(QString::number(cube->getC()));

	ui->cubeTexFront->addItems(*TextureManager::getTexturePaths());
	ui->cubeTexLeft->addItems(*TextureManager::getTexturePaths());
	ui->cubeTexBack->addItems(*TextureManager::getTexturePaths());
	ui->cubeTexRight->addItems(*TextureManager::getTexturePaths());
	ui->cubeTexBottom->addItems(*TextureManager::getTexturePaths());
	ui->cubeTexTop->addItems(*TextureManager::getTexturePaths());

	ui->cubeTexFront->setCurrentIndex(part->getTextureRefs()[0]);
	ui->cubeTexLeft->setCurrentIndex(part->getTextureRefs()[1]);
	ui->cubeTexBack->setCurrentIndex(part->getTextureRefs()[2]);
	ui->cubeTexRight->setCurrentIndex(part->getTextureRefs()[3]);
	ui->cubeTexBottom->setCurrentIndex(part->getTextureRefs()[4]);
	ui->cubeTexTop->setCurrentIndex(part->getTextureRefs()[5]);

	ui->originX->setText(QString::number(part->getOriginX()));
	ui->originY->setText(QString::number(part->getOriginY()));
	ui->originZ->setText(QString::number(part->getOriginZ()));

	ui->rotationX->setText(QString::number(part->getRotationX()));
	ui->rotationY->setText(QString::number(part->getRotationY()));
	ui->rotationZ->setText(QString::number(part->getRotationZ()));
#pragma endregion

#pragma region 事件绑定
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
				part->setMesh(newCube);
				ui->poleScene->updatePart(ui->drawableList->currentItem()->data(Qt::UserRole).value<PartDrawable *>());
				ui->poleScene->update();
			});
	ui->cubeB->disconnect();
	connect(ui->cubeB, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Cube *oldCube = (Cube *)part->getMesh();
				Cube *newCube = Cube::create(oldCube->getA(), text.toFloat(), oldCube->getC());
				part->setMesh(newCube);
				ui->poleScene->updatePart(ui->drawableList->currentItem()->data(Qt::UserRole).value<PartDrawable *>());
				ui->poleScene->update();
			});
	ui->cubeC->disconnect();
	connect(ui->cubeC, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				Cube *oldCube = (Cube *)part->getMesh();
				Cube *newCube = Cube::create(oldCube->getA(), oldCube->getB(), text.toFloat());
				part->setMesh(newCube);
				ui->poleScene->updatePart(ui->drawableList->currentItem()->data(Qt::UserRole).value<PartDrawable *>());
			}
	);

	// 纹理
	QComboBox *comboBoxes[6] = {
		ui->cubeTexFront,
		ui->cubeTexLeft,
		ui->cubeTexBack,
		ui->cubeTexRight,
		ui->cubeTexBottom,
		ui->cubeTexTop};
	for (int i = 0; i < 6; i++)
	{
		comboBoxes[i]->disconnect();
		connect(comboBoxes[i], &QComboBox::currentIndexChanged,
				[=](int index)
				{
					part->setTextureRef(i, index);
					ui->poleScene->update();
				});
	}

	// 变换
	ui->originX->disconnect();
	connect(ui->originX, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setOriginX(text.toFloat());
				ui->poleScene->update();
			});
	ui->originY->disconnect();
	connect(ui->originY, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setOriginY(text.toFloat());
				ui->poleScene->update();
			});
	ui->originZ->disconnect();
	connect(ui->originZ, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setOriginZ(text.toFloat());
				ui->poleScene->update();
			});
	ui->rotationX->disconnect();
	connect(ui->rotationX, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setRotationX(text.toFloat());
				ui->poleScene->update();
			});
	ui->rotationY->disconnect();
	connect(ui->rotationY, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setRotationY(text.toFloat());
				ui->poleScene->update();
			});
	ui->rotationZ->disconnect();
	connect(ui->rotationZ, &QLineEdit::textEdited,
			[=](const QString &text)
			{
				part->setRotationZ(text.toFloat());
				ui->poleScene->update();
			});
#pragma endregion
}

void PoleDesigner::bindCylinder(Part *part)
{
	ui->paramPanel->setCurrentWidget(ui->cylinderPanel);
	ui->cylinderName->setText(part->getName().c_str());
}

void PoleDesigner::bindStiffener(Part *part)
{
	ui->paramPanel->setCurrentWidget(ui->stiffenerPanel);
	ui->stiffenerName->setText(part->getName().c_str());
}

void PoleDesigner::bindRoundedCube(Part *part)
{
	ui->paramPanel->setCurrentWidget(ui->rCubePanel);
	ui->rCubeName->setText(part->getName().c_str());
}

void PoleDesigner::bindRoundedRegTriPrism(Part *part)
{
	ui->paramPanel->setCurrentWidget(ui->rRegTriPrismPanel);
	ui->rRegTriPrismName->setText(part->getName().c_str());
}

void PoleDesigner::hidePanels()
{
	ui->transformPanel->setVisible(false);
	ui->paramPanel->setCurrentWidget(ui->empty);
}

void PoleDesigner::initializeLineEdits()
{
	doubleValidator.setRange(-1000000000, 1000000000, 5);
	doubleValidator.setNotation(QDoubleValidator::StandardNotation);

	ui->originX->setValidator(&doubleValidator);
	ui->originY->setValidator(&doubleValidator);
	ui->originZ->setValidator(&doubleValidator);
	ui->rotationX->setValidator(&doubleValidator);
	ui->rotationY->setValidator(&doubleValidator);
	ui->rotationZ->setValidator(&doubleValidator);

	ui->cubeA->setValidator(&doubleValidator);
	ui->cubeB->setValidator(&doubleValidator);
	ui->cubeC->setValidator(&doubleValidator);

	ui->cylinderD->setValidator(&doubleValidator);
	ui->cylinderH->setValidator(&doubleValidator);

	ui->rCubeA->setValidator(&doubleValidator);
	ui->rCubeB->setValidator(&doubleValidator);
	ui->rCubeC->setValidator(&doubleValidator);
	ui->rCubeRadius->setValidator(&doubleValidator);

	ui->rRegTriPrismA->setValidator(&doubleValidator);
	ui->rRegTriPrismH->setValidator(&doubleValidator);
	ui->rRegTriPrismRadius->setValidator(&doubleValidator);

	ui->stiffenerA->setValidator(&doubleValidator);
	ui->stiffenerB->setValidator(&doubleValidator);
	ui->stiffenerC->setValidator(&doubleValidator);
	ui->stiffenerH1->setValidator(&doubleValidator);
	ui->stiffenerH2->setValidator(&doubleValidator);
	ui->stiffenerHOff->setValidator(&doubleValidator);
	ui->stiffenerVOff->setValidator(&doubleValidator);
}
