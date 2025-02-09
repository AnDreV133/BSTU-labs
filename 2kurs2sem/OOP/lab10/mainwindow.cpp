#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QString>
#include <QLabel>
#include <QPixmap>
#include <QWindow>
#include <QWidget>

#include <memory>
#include <iostream>
#include <exception>

#include "SmartPointer.hpp"
#include "FileSaver.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fs = new FileSaver("cache.txt");
    ui->comboBox->addItems(fs->getPathesToImages());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_reviewButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(
        this,
        QString(),
        QString(),
        "jpeg (*.jpeg);;bmp (*.bmp);;png (*.png)"
    );

    SmartPointer<Viewer> v = new Viewer(fileName.toStdString());
    ui->comboBox->setCurrentText(fileName);

    fs->save(*v);

    ui->comboBox->clear();
    ui->comboBox->addItems(fs->getPathesToImages());
}

void MainWindow::on_showButton_clicked()
{
    widget = new QWidget();

    QLabel *label = new QLabel();
    QString path = ui->comboBox->currentText();

    qDebug(path.toStdString().c_str());

    label->setText(path);
    label->setPixmap(QPixmap(path));
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(label);

    widget->setLayout(layout);
    widget->show();
}
