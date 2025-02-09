#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "Viewer.h"
#include "SmartPointer.hpp"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FileSaver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_showButton_clicked();
    void on_reviewButton_clicked();

private:
    Ui::MainWindow *ui;
    QString fileName;
    QWidget* widget;
    FileSaver* fs;
};

#endif // MAINWINDOW_H
