#pragma once

#include <ui_DBDialog.h>

class DBDialog : public QDialog 
{
    Q_OBJECT

    Ui::DBDialog* ui;
public:
    DBDialog(QVector<QPair<int, QString>>, QWidget *parent = nullptr);
    ~DBDialog();

signals:
    void openBtnClicked(int id);
    void deleteBtnClicked(int id);
};