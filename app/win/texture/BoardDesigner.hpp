#pragma once


#include <ui_BoardDesigner.h>

#include <QRegularExpressionValidator>

class BoardDesigner : public QWidget {

    Q_OBJECT

    Ui::BoardDesigner* ui;

    void initTemplateSelector();
    void initValidators();

public slots:
    void onExportBtnClicked();
    void onSelectionChanged(int index);
    void onTextEdited(const QString& text);
    void onTemplateSelected(const QString& templateName);
public:
    BoardDesigner(QWidget* parent = nullptr);
    ~BoardDesigner() override;

private:
    QRegularExpressionValidator characterValidator;
    QRegularExpressionValidator pinyinValidator;
    QRegularExpressionValidator letterValidator;
    QRegularExpressionValidator letterNumberValidator;
    QRegularExpressionValidator unitValidator;
};