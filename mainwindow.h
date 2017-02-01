#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QThread>

#include <QHotkey>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QHotkey* shortcut;

private slots:
    void on_hideButton_clicked();

    void on_shortcut_activated();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
