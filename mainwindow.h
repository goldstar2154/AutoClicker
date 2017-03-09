#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QHotkey>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QMenu>
#include "clicker.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_shortcut_activated();
    void on_pushButton_Exit_clicked();
    void lockForm();
    void unlockForm();
    void on_keySequenceEdit_ShortCut_editingFinished();
    void on_progress(const long& _progress);
    void on_pushButton_Hide_clicked();
    void on_trayShow();

signals:
    void setupWorker(const long& _count, const long& _delay, const int& _clickType, const int& _clickMode, const bool& _freezePointer);
    void setKeepGoing(const bool& _keepGoing);

private:
    Ui::MainWindow *ui;
    QHotkey* shortcut;
    QThread* clickerThread;
    Clicker* clickerWorker;
    QSettings settings;
    QSystemTrayIcon* sysTrayIcon;

    void saveSettings();
    void loadSettings();
};

#endif // MAINWINDOW_H
