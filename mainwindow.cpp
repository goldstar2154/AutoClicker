#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    shortcut(new QHotkey(this))
{
    ui->setupUi(this);

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
}

MainWindow::~MainWindow()
{
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
}

void MainWindow::unlockForm()
{
    setEnabled(true);
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
