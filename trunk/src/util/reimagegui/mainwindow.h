#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QProcess;

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
    void handleOutput();
    void on_actionUpload_firmware_triggered();
    void on_actionExit_triggered();

private:

    Ui::MainWindow *m_ui;
    QProcess *m_process;
};

#endif // MAINWINDOW_H
