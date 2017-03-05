#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString s_appName("Autoclicker");
const QString s_hotkey("clicker/hotkey");
const QString s_delay("clicker/delay");
const QString s_count("clicker/count");
const QString s_clickBtn("clicker/clickBtn");
const QString s_clickMode("clicker/clickMode");
const QString s_freezePointer("clicker/freezePointer");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    shortcut(new QHotkey(this)),
    settings(QSettings::IniFormat, QSettings::UserScope, s_appName, s_appName)
{
    ui->setupUi(this);

    sysTrayIcon = new QSystemTrayIcon(this);
    sysTrayIcon->setIcon(QIcon(":/icons/icon_idle"));

    QMenu* menu = new QMenu(this);
    QAction* actionShow = new QAction("Show", this);
    QAction* actionExit = new QAction("Exit", this);

    sysTrayIcon->setContextMenu(menu);

    menu->addAction(actionShow);
    menu->addAction(actionExit);

    connect(actionShow, SIGNAL(triggered()), this, SLOT(on_trayShow()));
    connect(actionExit, SIGNAL(triggered()), this, SLOT(close()));

    clickerThread = new QThread();
    clickerWorker = new Clicker();

    // Connecting objects
    connect(shortcut, SIGNAL(activated()), this, SLOT(on_shortcut_activated()));

    connect(clickerThread, SIGNAL(started()), this, SLOT(lockForm()));
    connect(clickerThread, SIGNAL(finished()), this, SLOT(unlockForm()));

    connect(clickerThread, SIGNAL(started()), clickerWorker, SLOT(start()));
    connect(clickerWorker, SIGNAL(finished()), clickerThread, SLOT(quit()));
    connect(clickerWorker, SIGNAL(progress(long)), this, SLOT(on_progress(long)));

    clickerWorker->moveToThread(clickerThread);

    // Setup validators
    ui->lineEdit_Delay->setValidator(new QIntValidator(0, 100000000, this));
    ui->lineEdit_Count->setValidator(new QIntValidator(0, 100000000, this));

    // Initializing combo-boxes
    ui->comboBox_ClickType->addItem("Left", QVariant(1));
    ui->comboBox_ClickType->addItem("Mid", QVariant(2));
    ui->comboBox_ClickType->addItem("Right", QVariant(3));

    ui->comboBox_ClickMode->addItem("Single", QVariant(1));
    ui->comboBox_ClickMode->addItem("Double", QVariant(2));

    loadSettings();
}

MainWindow::~MainWindow()
{
    saveSettings();

    if (clickerThread != nullptr && clickerThread->isRunning())
        clickerThread->exit();

    if (clickerThread != nullptr)
        delete clickerThread;

    if (clickerWorker != nullptr)
        delete clickerWorker;

    delete ui;
}

void MainWindow::lockForm()
{
    setEnabled(false);

    if ( this->isHidden() )
        sysTrayIcon->setIcon(QIcon(":/icons/icon_work"));
}

void MainWindow::unlockForm()
{
    setEnabled(true);

    if ( this->isHidden() )
        sysTrayIcon->setIcon(QIcon(":/icons/icon_idle"));
}

void MainWindow::on_progress(const long &_progress)
{
    ui->label_progress->setText(QString::number(_progress));
}

void MainWindow::on_shortcut_activated()
{
    if (clickerThread->isRunning())
    {
        clickerWorker->setKeepGoing(false);
    }
    else
    {
        clickerWorker->setup(ui->lineEdit_Count->text().toLong(),
                             ui->lineEdit_Delay->text().toLong(),
                             ui->comboBox_ClickType->itemData(ui->comboBox_ClickType->currentIndex()).toInt(),
                             ui->comboBox_ClickMode->itemData(ui->comboBox_ClickMode->currentIndex()).toInt(),
                             ui->checkBox_Freeze->isChecked());
        clickerWorker->setKeepGoing(true);

        clickerThread->start();
    }
}

void MainWindow::on_pushButton_Exit_clicked()
{
    this->close();
}

void MainWindow::on_keySequenceEdit_ShortCut_editingFinished()
{
    shortcut->setShortcut(ui->keySequenceEdit_ShortCut->keySequence(), true);
}

void MainWindow::saveSettings()
{
    settings.setValue(s_hotkey, ui->keySequenceEdit_ShortCut->keySequence().toString());
    settings.setValue(s_delay, ui->lineEdit_Delay->text());
    settings.setValue(s_count, ui->lineEdit_Count->text());
    settings.setValue(s_clickBtn, ui->comboBox_ClickType->itemData(ui->comboBox_ClickType->currentIndex()).toInt() - 1);
    settings.setValue(s_clickMode, ui->comboBox_ClickMode->itemData(ui->comboBox_ClickMode->currentIndex()).toInt() - 1);
    settings.setValue(s_freezePointer, ui->checkBox_Freeze->isChecked());
}

void MainWindow::loadSettings()
{
    ui->keySequenceEdit_ShortCut->setKeySequence(QKeySequence(settings.value(s_hotkey).toString()));
    ui->lineEdit_Delay->setText(settings.value(s_delay).toString());
    ui->lineEdit_Count->setText(settings.value(s_count).toString());
    ui->comboBox_ClickType->setCurrentIndex(settings.value(s_clickBtn).toInt());
    ui->comboBox_ClickMode->setCurrentIndex(settings.value(s_clickMode).toInt());
    ui->checkBox_Freeze->setChecked(settings.value(s_freezePointer).toBool());

    // need to emit for QHotkey
    emit on_keySequenceEdit_ShortCut_editingFinished();
}

void MainWindow::on_pushButton_Hide_clicked()
{
    hide();
    sysTrayIcon->show();
}

void MainWindow::on_trayShow()
{
    show();
    sysTrayIcon->hide();
}
