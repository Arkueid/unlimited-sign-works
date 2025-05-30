#include "DBDialog.hpp"
#include <QMessageBox>
DBDialog::DBDialog(QVector<QPair<int, QString>> values, QWidget *parent): QDialog(parent), ui(new Ui::DBDialog)
{
    ui->setupUi(this);

    for (auto& pair : values)
    {
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        item->setData(Qt::UserRole, pair.first);
        item->setText(pair.second);
        ui->listWidget->addItem(item);
    }

    connect(ui->deleteBtn, &QPushButton::clicked, this, [this](){
        const QListWidgetItem* item = ui->listWidget->currentItem();
        if (item == nullptr) return;

        if (QMessageBox::question(this, "删除" + this->windowTitle(), "是否删除" + item->text() + "？", QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        {
            return;
        }
        
        emit deleteBtnClicked(item->data(Qt::UserRole).toInt());

        ui->listWidget->takeItem(ui->listWidget->row(item));
    });
    connect(ui->openBtn, &QPushButton::clicked, this, [this](){
        const QListWidgetItem* item = ui->listWidget->currentItem();
        if (item == nullptr) return;
        emit openBtnClicked(item->data(Qt::UserRole).toInt());
        this->close();
    });
}

DBDialog::~DBDialog()
{
    delete ui;
}
