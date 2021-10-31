#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_gcodeBtn_clicked();

    void on_execBtn_clicked();

private:
    Ui::MainWindow *ui;

    QString gcodeFileName;
    QString gcodeDefaultDirName; //默认的gcode文件夹目录
    QTcpSocket* tcpSock = nullptr;
};
#endif // MAINWINDOW_H
