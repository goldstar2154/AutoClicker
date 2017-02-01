#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    shortcut(new QHotkey(this))
{
    ui->setupUi(this);

//    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(on_shortcut_activated()));



     shortcut->setShortcut(QKeySequence("Ctrl+O"), true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_hideButton_clicked()
{

}

void MainWindow::on_shortcut_activated()
{
    std::cout << "fff" << std::endl;
}
