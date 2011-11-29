#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPlainTextEdit>
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    setWindowTitle("VEXPro ARM9 Reimager");

    m_process = NULL;
}

MainWindow::~MainWindow()
{
    if (m_process)
    {
        m_process->kill();
        delete m_process;
    }
    delete m_ui;
}


void MainWindow::handleOutput()
{
    QByteArray output = m_process->readAllStandardOutput();
    if (output.data())
        m_ui->output->insertPlainText(output.data());

    QByteArray error = m_process->readAllStandardError();
    if (error.data())
        m_ui->output->insertPlainText(error.data());
}

void MainWindow::on_actionUpload_firmware_triggered()
{
    if (m_process)
    {
        m_process->kill();
        delete m_process;
    }

    m_ui->output->clear();

    QString firmware = QFileDialog::getOpenFileName(this, "Select firmware file", "", "*.dat");

    if (firmware=="")
        return;

    QMessageBox::information(this, "Reimager", "1. Connect \"Console\" port on your VEXPro ARM9\nto a USB port on your computer.\n\n2. Follow directions in window.");


    QString program = "reimage.exe";
    QStringList arguments;
    arguments << firmware;

    m_process = new QProcess(this);
    m_process->start(program, arguments);

    connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(handleOutput()));
    connect(m_process, SIGNAL(aboutToClose()), this, SLOT(handleOutput()));
    connect(m_process, SIGNAL(readyReadStandardError()), this, SLOT(handleOutput()));

}

void MainWindow::on_actionExit_triggered()
{
    close();
}
