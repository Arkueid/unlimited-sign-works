#include <win/PoleDesigner.hpp>
#include <QListWidgetItem>
#include <win/list/DrawableListItem.hpp>
#include <win/panel/CubePanel.hpp>
#include <QMenu>

PoleDesigner::PoleDesigner(QWidget *parent) : QWidget(parent),
                                              ui(new Ui::PoleDesigner),
											  cube(nullptr),
											  cylinder(nullptr),
											  stiffener(nullptr),
											  roundedCube(nullptr),
											  rrTriPrism(nullptr)
{
	ui->setupUi(this);
	ui->leftSplitter->setStretchFactor(0, 1);
	ui->leftSplitter->setStretchFactor(1, 8);
	ui->rightSplitter->setStretchFactor(0, 8);
	ui->rightSplitter->setStretchFactor(1, 1);

	resize(1280, 720);

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


void PoleDesigner::bindPart(Part * part)
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

void PoleDesigner::bindCube(Part* part)
{
	ui->paramPanel->setCurrentWidget(ui->cubePanel);
	ui->cubeName->setText(part->getName().c_str());
	cube = (Cube*)part->getMesh();

	ui->cubeA->setText(QString::number(cube->getA()));
	ui->cubeB->setText(QString::number(cube->getB()));
	ui->cubeC->setText(QString::number(cube->getC()));

	// ui->cubeTexFront->addItems();
}

void PoleDesigner::bindCylinder(Part* part)
{
	ui->paramPanel->setCurrentWidget(ui->cylinderPanel);
	ui->cylinderName->setText(part->getName().c_str());
}

void PoleDesigner::bindStiffener(Part* part)
{
	ui->paramPanel->setCurrentWidget(ui->stiffenerPanel);
	ui->stiffenerName->setText(part->getName().c_str());
}

void PoleDesigner::bindRoundedCube(Part* part)
{
	ui->paramPanel->setCurrentWidget(ui->rCubePanel);
	ui->rCubeName->setText(part->getName().c_str());
}

void PoleDesigner::bindRoundedRegTriPrism(Part* part)
{
	ui->paramPanel->setCurrentWidget(ui->rRegTriPrismPanel);
	ui->rRegTriPrismName->setText(part->getName().c_str());
}
