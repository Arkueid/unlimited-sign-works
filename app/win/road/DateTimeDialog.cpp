#include "DateTimeDialog.hpp"

DateTimeDialog::DateTimeDialog(QWidget *parent): QDialog(parent), ui(new Ui::DateTimeDialog)
{
    ui->setupUi(this);

    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    adjustSize();
}

DateTimeDialog::~DateTimeDialog()
{
    delete ui;
}

const QDateTime DateTimeDialog::getDateTime() const
{
    return ui->dateTimeEdit->dateTime();
}
