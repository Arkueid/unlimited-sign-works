#pragma once

#include <ui_MainWindow.h>

#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;

public slots:
    void onNewRoadRequested();
    void onOpenRoadRequested();

    void onNewPoleRequested();
    void onOpenPoleRequested();

    void onTexGenRequested();
    void onTexBaseRequested();

    void onTabClosed(int index);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};