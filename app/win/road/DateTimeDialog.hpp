#pragma once

#include "ui_DateTimeDialog.h"

class DateTimeDialog: public QDialog
{
    Q_OBJECT

    Ui::DateTimeDialog* ui;
public:
    explicit DateTimeDialog(QWidget* parent = nullptr);
    ~DateTimeDialog();

    const QDateTime getDateTime() const;
};