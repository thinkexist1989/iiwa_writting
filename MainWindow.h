#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QTcpSocket>
#include <QPointF>

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
    void on_gcode1Btn_clicked();

    void on_execBtn_clicked();

    void on_stickFontBtn_clicked();

    void on_modeBox_currentIndexChanged(int index);

    void on_gcode2Btn_clicked();

    void on_selectDir_clicked();

    void on_connectBtn_clicked();

private:
    Ui::MainWindow *ui;

    QString gcode1FileName;
    QString gcode2FileName;

    QString sigFileName = "bySIA.nc";

    QString rootDirName;

    QString gcodeDefaultDirName; //默认的gcode文件夹目录
    QTcpSocket* tcpSock = nullptr;

    QPointF line1Offset;
    QPointF line2Offset;
    QPointF nameOffset;
    QPointF sigOffset;

    int mode = 0; // 0 是2line 1是name
    void send_gcode(QString fileName, QPointF origin, QTcpSocket* tcpSocket);
};
#endif // MAINWINDOW_H
