#include <win/PoleDesigner.hpp>
#include <QListWidgetItem>
#include <win/list/DrawableListItem.hpp>
#include <win/panel/CubePanel.hpp>
#include <QMenu>

PoleDesigner::PoleDesigner(QWidget *parent) : QWidget(parent), ui(new Ui::PoleDesigner)
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
					qDebug() << "Removing drawable: " << ui->drawableList->currentRow();
					QListWidgetItem *item = ui->drawableList->takeItem(ui->drawableList->currentRow());
					ui->poleScene->removeDrawable(item->data(Qt::UserRole).value<PartDrawable *>());
					delete item;
				}
			});
}

PoleDesigner::~PoleDesigner()
{
	delete ui;
}
